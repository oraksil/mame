#include "emu.h"
#include "modules/lib/osdobj_common.h"
#include "modules/input/input_headless.h"
#include "headless.h"
#include "headless_osd.h"

const int COLOR_DEPTH_BYTES = 4;

static mame_headless_t *s_instance;

static headless_osd_interface *s_osd;
static osd_options *s_options;
static mame_frame_callback_t s_frame_cb;
static mame_frame_t s_frame;
static image_buffer_info_t s_buf_info;

static void osd_update_cb(bool skip_redraw)
{
    if (s_frame_cb != nullptr) {
        s_frame_cb(s_frame);
    }
}

static void headless_init(mame_frame_info_t &frame_info, mame_frame_callback_t cb)
{
    assert(s_osd == nullptr);
    assert(s_options == nullptr);
    assert(s_frame_cb == nullptr);

    char resolution[16];
    sprintf(resolution, "%dx%d@60", frame_info.width, frame_info.height);
    s_options = new osd_options();
    s_options->set_value(OSD_MONITOR_PROVIDER, "headless", OPTION_PRIORITY_MAXIMUM);
    s_options->set_value(OSD_KEYBOARDINPUT_PROVIDER, "headless", OPTION_PRIORITY_MAXIMUM);
    s_options->set_value(OSDOPTION_SCREEN, OSDOPTVAL_AUTO, OPTION_PRIORITY_MAXIMUM);
    s_options->set_value(OSDOPTION_WINDOW, 1, OPTION_PRIORITY_MAXIMUM);
    s_options->set_value(OSDOPTION_RESOLUTION, resolution, OPTION_PRIORITY_MAXIMUM);

    s_frame.buf_size = frame_info.width * frame_info.height * COLOR_DEPTH_BYTES;
    s_frame.buffer = new uint8_t[s_frame.buf_size];
    s_frame_cb = cb;

    s_buf_info.width = frame_info.width;
    s_buf_info.height = frame_info.height;
    s_buf_info.buffer = s_frame.buffer;

    s_osd = new headless_osd_interface(*s_options);
    s_osd->register_options();
    s_osd->set_buffer_info(&s_buf_info);
    s_osd->set_update_callback(osd_update_cb);
}

static void headless_enqueue_input_evt(mame_input_event_t input_event)
{
    assert(s_osd != nullptr);
    assert(s_options != nullptr);

    input_module *keyboard_input = s_osd->input();
    headless_input_module *mod = dynamic_cast<headless_input_module*>(keyboard_input);
    if (mod == nullptr) {
        return;
    }

    HeadlessKeyEvent event;
    event.ascii_key = input_event.key;
    event.event_id = input_event.type == INPUT_KEY_DOWN ?
        HEADLESS_EVENT_KEYDOWN : HEADLESS_EVENT_KEYUP;

    mod->handle_input_event(event);
}

static int headless_run(const char *system_name)
{
    assert(s_osd != nullptr);
    assert(s_options != nullptr);

    std::vector<std::string> dummy_args;

    s_options->set_system_name(system_name);

    return emulator_info::start_frontend(*s_options, *s_osd, dummy_args);
}

mame_headless_t* get_mame_headless(mame_frame_info_t frame_info, mame_frame_callback_t cb)
{
    if (s_instance != nullptr) {
        return s_instance;
    }

    s_instance = new mame_headless_t();
    s_instance->enqueue_input_event = headless_enqueue_input_evt;
    s_instance->run = headless_run;

    headless_init(frame_info, cb);

    return s_instance;
}

void on_mame_machine_setup()
{
}