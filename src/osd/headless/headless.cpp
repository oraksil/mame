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
    osd_options *opts;
    headless_osd_interface *osd;
    image_frame_buf_info_t image_buf_info;

    mame_image_frame_t image_frame;
    mame_image_frame_cb_t image_frame_cb;
    void *image_frame_cb_ctx;

    mame_sound_frame_cb_t sound_frame_cb;
    void *sound_frame_cb_ctx;
} mame_internal_t;

static mame_internal_t *s_inst;

static void osd_frame_cb(bool skip_redraw)
{
    assert(s_inst != nullptr);
    assert(s_inst->image_frame_cb != nullptr);

    s_inst->image_frame_cb(s_inst->image_frame_cb_ctx, s_inst->image_frame);
}

static void osd_sound_cb(sound_frame_buf_info_t buf)
{
    assert(s_inst != nullptr);
    assert(s_inst->sound_frame_cb != nullptr);

    mame_sound_frame_t snd_buf = { .buffer = buf.buffer, .buf_size = buf.buf_size };
    s_inst->sound_frame_cb(s_inst->sound_frame_cb_ctx, snd_buf);
}

static void headless_init()
{
    assert(s_inst != nullptr);
    assert(s_inst->osd == nullptr);
    assert(s_inst->opts == nullptr);
    assert(s_inst->image_frame_cb == nullptr);
    assert(s_inst->sound_frame_cb == nullptr);

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

    if (s_inst->image_frame.buffer != nullptr)
        delete s_inst->image_frame.buffer;
    s_inst->image_frame.buf_size = w * h * COLOR_DEPTH_BYTES;
    s_inst->image_frame.buffer = new uint8_t[s_inst->image_frame.buf_size];

    s_inst->image_buf_info.width = w;
    s_inst->image_buf_info.height = h;
    s_inst->image_buf_info.buffer = s_inst->image_frame.buffer;

    s_inst->osd->set_image_buffer_info(&s_inst->image_buf_info);
}

static void headless_set_image_frame_cb(void *ctx, mame_image_frame_cb_t frame_cb)
{
    assert(s_inst != nullptr);
    assert(s_inst->osd != nullptr);

    s_inst->image_frame_cb = frame_cb;
    s_inst->image_frame_cb_ctx = ctx;
    s_inst->osd->set_image_frame_cb(osd_frame_cb);
}

static void headless_set_sound_frame_cb(void *ctx, mame_sound_frame_cb_t frame_cb)
{
    assert(s_inst != nullptr);
    assert(s_inst->osd != nullptr);

    s_inst->sound_frame_cb = frame_cb;
    s_inst->sound_frame_cb_ctx = ctx;

    s_inst->osd->set_sound_frame_cb(osd_sound_cb);
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
    s_inst->mame->set_image_frame_info = headless_set_frame_info;
    s_inst->mame->set_image_frame_cb = headless_set_image_frame_cb;
    s_inst->mame->set_sound_frame_cb = headless_set_sound_frame_cb;;
    s_inst->mame->enqueue_input_event = headless_enqueue_input_evt;
    s_inst->mame->run = headless_run;

    headless_init();

    return s_inst->mame;
}

void on_mame_machine_setup()
{
}