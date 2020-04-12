#include <cstdint>
#include <cstddef>

#ifndef MAME_HEADLESS_H
#define MAME_HEADLESS_H

extern "C" {
    typedef enum {
        INPUT_KEY_DOWN,
        INPUT_KEY_UP
    } mame_input_enum_t;

    typedef struct {
        uint8_t key;
        mame_input_enum_t type;
    } mame_input_event_t;

    typedef struct {
        uint8_t *buffer;
        size_t buf_size;
    } mame_image_frame_t;

    typedef struct {
        const int16_t *buffer;
        size_t buf_size;
    } mame_sound_frame_t;

    typedef void (*mame_image_frame_cb_t)(void *ctx, mame_image_frame_t frame);
    typedef void (*mame_sound_frame_cb_t)(void *ctx, mame_sound_frame_t frame);

    typedef struct {
        void (*set_image_frame_info)(int w, int h);
        void (*set_image_frame_cb)(void *ctx, mame_image_frame_cb_t frame_cb);
        void (*set_sound_frame_cb)(void *ctx, mame_sound_frame_cb_t frame_cb);
        void (*enqueue_input_event)(mame_input_event_t input_event);
        int (*run)(const char *system_name);
    } mame_t;

    mame_t* get_mame_instance();
}

#endif