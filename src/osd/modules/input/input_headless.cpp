
#include "input_module.h"
#include "modules/osdmodule.h"

// keyboard test
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
// end of keyboard test

#include <ctype.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <stddef.h>
#include <mutex>
#include <memory>
#include <queue>
#include <iterator>
#include <algorithm>

// MAME headers
#include "emu.h"
#include "uiinput.h"
#include "strconv.h"

// MAMEOS headers
#include "input_common.h"
#include "modules/lib/osdobj_common.h"

#include "input_headless.h"

// winnt.h defines this
#ifdef DELETE
#undef DELETE
#endif

// FIXME: sdl does not properly report the window for certain OS.
#define GET_FOCUS_WINDOW(ev) focus_window()

struct key_lookup_table
{
	int code;
	const char *name;
};

//============================================================
//  sdl_keyboard_device
//============================================================

class headless_keyboard_device : public event_based_device<HeadlessKeyEvent>
{
public:
	headless_keyboard_state keyboard;

	headless_keyboard_device(running_machine &machine, const char *name, const char *id, input_module &module)
		: event_based_device(machine, name, id, DEVICE_CLASS_KEYBOARD, module),
		  keyboard({{0}})
	{
	}

	void process_event(HeadlessKeyEvent &event) override
	{
		keyboard.state[event.ascii_key] = event.event_id == HEADLESS_EVENT_KEYDOWN ? 0x80 : 0x00;
	}

	void reset() override
	{
		memset(&keyboard.state, 0, sizeof(keyboard.state));
	}
};

//============================================================
//  headless_input_module
//============================================================

headless_input_module::headless_input_module(const char *type)
	: input_module_base(type, "headless")
{
}

void headless_input_module::input_init(running_machine &machine)
{
	if (machine.debug_flags & DEBUG_FLAG_OSD_ENABLED)
	{
		osd_printf_warning("Debug Build: Disabling input grab for -debug\n");
	}
}

void headless_input_module::exit()
{
	input_module_base::exit();
}

void headless_input_module::handle_input_event(HeadlessKeyEvent &key)
{
	devicelist()->for_each_device([key](auto device) {
		downcast<headless_keyboard_device *>(device)->queue_events(&key, 1);
	});
}

bool headless_input_module::should_poll_devices(running_machine &machine)
{
	return true;
}

//============================================================
//  sdl_keyboard_module
//============================================================

#define KEY_TRANS_ENTRY0(mame, sdlsc, sdlkey, disc, virtual, uwp, ascii, UI) \
	{                                                                        \
		ITEM_ID_##mame, ascii, "ITEM_ID_"#mame, (char *)UI           		 \
	}
#define KEY_TRANS_ENTRY1(mame, sdlsc, sdlkey, disc, virtual, uwp, ascii)     \
	{                                                                        \
		ITEM_ID_##mame, ascii, "ITEM_ID_"#mame, (char *)#mame                \
	}

headless_keyboard_module::headless_keyboard_module()
	: headless_input_module(OSD_KEYBOARDINPUT_PROVIDER)
{
}

void headless_keyboard_module::setup_keytable(keyboard_trans_table &table)
{
}

void headless_keyboard_module::before_poll(running_machine &machine)
{
}

void headless_keyboard_module::input_init(running_machine &machine)
{
	headless_input_module::input_init(machine);
	headless_keyboard_device *devinfo;

	key_trans_entry new_table[] = {
		KEY_TRANS_ENTRY0(ESC, ESCAPE, ESCAPE, ESCAPE, VK_ESCAPE, Escape, 27, "ESCAPE"),
		KEY_TRANS_ENTRY1(DOWN, DOWN, DOWN, DOWN, VK_DOWN, Down, 40),
		KEY_TRANS_ENTRY1(UP, UP, UP, UP, VK_UP, Up, 38),
		KEY_TRANS_ENTRY1(LEFT, LEFT, LEFT, LEFT, VK_LEFT, Left, 37),
		KEY_TRANS_ENTRY1(RIGHT, RIGHT, RIGHT, RIGHT, VK_RIGHT, Right, 39),
		KEY_TRANS_ENTRY1(1, 1, 1, 1, '1', Number1, 49),
		KEY_TRANS_ENTRY1(5, 5, 5, 5, '5', Number5, 53),
		KEY_TRANS_ENTRY1(LCONTROL, LCTRL, LCTRL, LCONTROL, VK_LCONTROL, LeftControl, 17),
		KEY_TRANS_ENTRY1(LALT, LALT, LALT, LMENU, VK_LMENU, LeftMenu, 18),

		// player2
		KEY_TRANS_ENTRY1(A, A, a, A, 'A', A, 'A'),
		KEY_TRANS_ENTRY1(S, S, s, S, 'S', S, 'S'),
		KEY_TRANS_ENTRY1(2, 2, 2, 2, '2', 2, '2'),
		KEY_TRANS_ENTRY1(6, 6, 6, 6, '6', 6, '6'),
		KEY_TRANS_ENTRY1(R, R, r, R, 'R', R, 'R'),
		KEY_TRANS_ENTRY1(F, F, f, F, 'F', F, 'F'),
		KEY_TRANS_ENTRY1(D, D, d, D, 'D', D, 'D'),
		KEY_TRANS_ENTRY1(G, G, g, G, 'G', G, 'G'),

		KEY_TRANS_ENTRY0(INVALID, UNKNOWN, UNKNOWN, ESCAPE, 0, None, 0, "INVALID")};

	auto key_trans_entries = std::make_unique<key_trans_entry[]>(18);
	for (int i = 0; i < 18; i++)
		key_trans_entries[i] = new_table[i];

	// keyboard_trans_table table(std::move(key_trans_entries), (unsigned int)4);
	keyboard_trans_table *table = auto_alloc(machine, keyboard_trans_table(std::move(key_trans_entries), 18));
	osd_printf_verbose("Keyboard: Start initialization\n");

	// Oraksil has keyboard only
	devinfo = devicelist()->create_device<headless_keyboard_device>(machine, "System keyboard", "System keyboard", *this);

	// populate it
	for (int keynum = 0; (*table)[keynum].mame_key != ITEM_ID_INVALID; keynum++)
	{
		input_item_id itemid = (*table)[keynum].mame_key;

		// generate the default / modified name
		char defname[20];
		snprintf(defname, sizeof(defname) - 1, "%s", (*table)[keynum].ui_name);

		devinfo->device()->add_item(defname, itemid, generic_button_get_state<std::int32_t>, &devinfo->keyboard.state[(int)((*table)[keynum].ascii_key)]);
	}

	osd_printf_verbose("Keyboard: Registered %s\n", devinfo->name());
	osd_printf_verbose("Keyboard: End initialization\n");
}

MODULE_DEFINITION(KEYBOARDINPUT_HEADLESS, headless_keyboard_module)
