#ifndef INPUT_HEADLESS_H_
#define INPUT_HEADLESS_H_

#include "input_common.h"

enum oraksil_event_id
{
	ORAKSIL_EVENT_KEYDOWN,
	ORAKSIL_EVENT_KEYUP,
};

struct OraksilEvent
{
    oraksil_event_id event_id;
	uint8_t vkey;
	uint8_t ascii_key;
};

struct oraksil_keyboard_state
{
	int32_t   state[MAX_KEYS];                                   // must be int32_t!
	int8_t    oldkey[MAX_KEYS];
	int8_t    currkey[MAX_KEYS];
};

// not surely sure about necessity of mouse_module for oraksil
class oraksil_input_module : public input_module_base
{
public:
	oraksil_input_module(const char *type);
	void input_init(running_machine &machine) override;
	void exit() override;
	void handle_input_event(OraksilEvent &key);
	bool should_poll_devices(running_machine &machine) override;
};

class oraksil_keyboard_module : public oraksil_input_module
{
public:
	oraksil_keyboard_module();
	void oraksil_setup_keytable(keyboard_trans_table &table);
	void before_poll(running_machine &machine) override;
	void input_init(running_machine &machine) override;
};

#endif  // INPUT_HEADLESS_H_