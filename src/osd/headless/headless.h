#include <cstdint>

#ifndef MAME_HEADLESS_H
#define MAME_HEADLESS_H

extern "C" {
    typedef struct {
        int width;
        int height;
    } mame_frame_info_t;

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

    typedef void (*mame_frame_callback_t)(mame_frame_t frame);

    typedef struct {
        void (*enqueue_input_event)(mame_input_event_t input_event);
        int (*run)(const char *system_name);
    } mame_headless_t;

    mame_headless_t* get_mame_headless(mame_frame_info_t frame_info, mame_frame_callback_t cb);
}

#endif