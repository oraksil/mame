#include <cstdint>

typedef void (*update_callback_t)(bool);

typedef struct {
    int width;
    int height;
    uint8_t *buffer;
} image_buffer_info_t;

// int osd_entrypoint(int argc, char** argv, image_buffer_info_t *buf_info, update_callback_t cb);
int osd_entrypoint(int argc, char** argv, image_buffer_info_t *buf_info, update_callback_t cb, void** retOSD);
void handle_input_event(void* osd, uint8_t ascii_key, bool keyDown);