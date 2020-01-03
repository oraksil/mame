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

    headless_input_module* mod = dynamic_cast<headless_input_module*>(keyboard_input);

    HeadlessKeyEvent event;

   	event.event_id = keyDown == true ? HEADLESS_EVENT_KEYDOWN : HEADLESS_EVENT_KEYUP;
	event.ascii_key = ascii_key;

    if (mod) {
        mod->handle_input_event(event);
    }
}