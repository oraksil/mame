#ifndef INPUT_HEADLESS_H_
#define INPUT_HEADLESS_H_

#include "input_common.h"

enum headless_event_id
{
	HEADLESS_EVENT_KEYDOWN,
	HEADLESS_EVENT_KEYUP,
};

struct HeadlessKeyEvent
{
    headless_event_id event_id;
	uint8_t vkey;
	uint8_t ascii_key;
};

struct headless_keyboard_state
{
	int32_t state[MAX_KEYS];                                   // must be int32_t!
	int8_t old_key[MAX_KEYS];
	int8_t curr_key[MAX_KEYS];
};

class headless_input_module : public input_module_base
{
public:
	headless_input_module(const char *type);
	void input_init(running_machine &machine) override;
	void exit() override;
	void handle_input_event(HeadlessKeyEvent &key);
	bool should_poll_devices(running_machine &machine) override;
};

class headless_keyboard_module : public headless_input_module
{
public:
	headless_keyboard_module();
	void setup_keytable(keyboard_trans_table &table);
	void before_poll(running_machine &machine) override;
	void input_init(running_machine &machine) override;
};

#endif  // INPUT_HEADLESS_H_