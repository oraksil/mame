#include "emu.h"
#include "modules/lib/osdobj_common.h"
#include "modules/input/input_headless.h"
#include "headless.h"
#include "headless_osd.h"

typedef struct {
    int width;
    int height;
} mame_frame_info_t;

typedef struct {
    mame_t *mame;

    mame_frame_t frame;
    mame_frame_callback_t frame_cb;
    void *frame_cb_ctx;

    osd_options *opts;
    headless_osd_interface *osd;
    image_buffer_info_t buf_info;
} mame_internal_t;

static mame_internal_t *s_inst;

static void osd_update_cb(bool skip_redraw)
{
    assert(s_inst != nullptr);
    assert(s_inst->frame_cb != nullptr);

    s_inst->frame_cb(s_inst->frame_cb_ctx, s_inst->frame);
}

static void headless_init()
{
    assert(s_inst != nullptr);
    assert(s_inst->osd == nullptr);
    assert(s_inst->opts == nullptr);
    assert(s_inst->frame_cb == nullptr);

    s_inst->opts = new osd_options();
    s_inst->opts->set_value(OSD_MONITOR_PROVIDER, "headless", OPTION_PRIORITY_MAXIMUM);
    s_inst->opts->set_value(OSD_KEYBOARDINPUT_PROVIDER, "headless", OPTION_PRIORITY_MAXIMUM);
    s_inst->opts->set_value(OSDOPTION_SCREEN, OSDOPTVAL_AUTO, OPTION_PRIORITY_MAXIMUM);
    s_inst->opts->set_value(OSDOPTION_WINDOW, 1, OPTION_PRIORITY_MAXIMUM);

    s_inst->osd = new headless_osd_interface(*s_inst->opts);
    s_inst->osd->register_options();
}

const int COLOR_DEPTH_BYTES = 4;
static void headless_set_frame_info(int w, int h)
{
    assert(s_inst != nullptr);
    assert(s_inst->osd != nullptr);
    assert(s_inst->opts != nullptr);

    char resolution[16];
    sprintf(resolution, "%dx%d@60", w, h);
    s_inst->opts->set_value(OSDOPTION_RESOLUTION, resolution, OPTION_PRIORITY_MAXIMUM);

    if (s_inst->frame.buffer != nullptr)
        delete s_inst->frame.buffer;
    s_inst->frame.buf_size = w * h * COLOR_DEPTH_BYTES;
    s_inst->frame.buffer = new uint8_t[s_inst->frame.buf_size];

    s_inst->buf_info.width = w;
    s_inst->buf_info.height = h;
    s_inst->buf_info.buffer = s_inst->frame.buffer;

    s_inst->osd->set_buffer_info(&s_inst->buf_info);
}

static void headless_set_frame_cb(void *ctx, mame_frame_callback_t frame_cb)
{
    assert(s_inst != nullptr);
    assert(s_inst->osd != nullptr);

    s_inst->frame_cb = frame_cb;
    s_inst->frame_cb_ctx = ctx;
    s_inst->osd->set_update_callback(osd_update_cb);
}

static void headless_set_sound_cb(mame_sound_callback_t sound_cb)
{
    assert(s_inst != nullptr);
    assert(s_inst->osd != nullptr);

    s_inst->osd->set_sound_callback(sound_cb);
}

static void headless_enqueue_input_evt(mame_input_event_t input_event)
{
    assert(s_inst != nullptr);
    assert(s_inst->osd != nullptr);

    input_module *keyboard_input = s_inst->osd->input();
    headless_input_module *mod = dynamic_cast<headless_input_module*>(keyboard_input);
    if (mod == nullptr)
        return;

    HeadlessKeyEvent event;
    event.ascii_key = input_event.key;
    event.event_id = input_event.type == INPUT_KEY_DOWN ?
        HEADLESS_EVENT_KEYDOWN : HEADLESS_EVENT_KEYUP;

    mod->handle_input_event(event);
}

static int headless_run(const char *system_name)
{
    assert(s_inst != nullptr);
    assert(s_inst->osd != nullptr);
    assert(s_inst->opts != nullptr);

    std::vector<std::string> dummy_args;

    s_inst->opts->set_system_name(system_name);

    return emulator_info::start_frontend(*s_inst->opts, *s_inst->osd, dummy_args);
}

mame_t* get_mame_instance()
{
    if (s_inst != nullptr && s_inst->mame != nullptr) {
        return s_inst->mame;
    }

    s_inst = new mame_internal_t();
    s_inst->mame = new mame_t();
    s_inst->mame->set_frame_info = headless_set_frame_info;
    s_inst->mame->set_frame_cb = headless_set_frame_cb;
    s_inst->mame->set_sound_cb = headless_set_sound_cb;;
    s_inst->mame->enqueue_input_event = headless_enqueue_input_evt;
    s_inst->mame->run = headless_run;

    headless_init();

    return s_inst->mame;
}

void on_mame_machine_setup()
{
}