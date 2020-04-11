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
    } mame_frame_t;

    typedef struct {
        const int16_t *buffer;
        size_t buf_size;
    } mame_sound_t;

    typedef void (*mame_frame_callback_t)(void *ctx, mame_frame_t frame);
    typedef void (*mame_sound_callback_t)(mame_sound_t sound);

    typedef struct {
        void (*set_frame_info)(int w, int h);
        void (*set_frame_cb)(void *ctx, mame_frame_callback_t frame_cb);
        void (*set_sound_cb)(mame_sound_callback_t frame_cb);
        void (*enqueue_input_event)(mame_input_event_t input_event);
        int (*run)(const char *system_name);
    } mame_t;

    mame_t* get_mame_instance();
}

#endif