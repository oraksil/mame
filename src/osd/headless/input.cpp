#include "emu.h"
#include "osdheadless.h"
#include "modules/input/input_headless.h"

//============================================================
//  input_update
//============================================================

void headless_osd_interface::input_update()
{
}

void handle_input_event(void* osd, uint8_t ascii_key, bool keyDown) {
    input_module* keyboard_input = ((headless_osd_interface*)osd)->input();

    oraksil_input_module* mod = dynamic_cast<oraksil_input_module*>(keyboard_input);

    OraksilEvent event;

    if (mod) {
        mod->handle_input_event(event);
    }
}