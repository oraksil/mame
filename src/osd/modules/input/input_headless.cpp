
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
		ITEM_ID_##mame, ascii, "ITEM_ID_" #mame, (char *)UI                  \
	}
#define KEY_TRANS_ENTRY1(mame, sdlsc, sdlkey, disc, virtual, uwp, ascii) \
	{                                                                    \
		ITEM_ID_##mame, ascii, "ITEM_ID_" #mame, (char *)#mame           \
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
		KEY_TRANS_ENTRY1(LCONTROL, LCTRL, LCTRL, LCONTROL, VK_LCONTROL, LeftControl, 90),
		KEY_TRANS_ENTRY1(LALT, LALT, LALT, LMENU, VK_LMENU, LeftMenu, 88),
		KEY_TRANS_ENTRY1(SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, 67),
		KEY_TRANS_ENTRY1(LSHIFT, LSHIFT, LSHIFT, LSHIFT, LSHIFT, LSHIFT, 65),
		KEY_TRANS_ENTRY1(Z, Z, z, Z, 'Z', Z, 83),
		KEY_TRANS_ENTRY1(X, X, x, X, 'X', X, 68), 
		KEY_TRANS_ENTRY1(C, C, c, C, 'C', C, 81), 
		KEY_TRANS_ENTRY1(V, V, v, V, 'V', V, 87), 


		// player2
		KEY_TRANS_ENTRY1(R, R, r, R, 'R', R, 1),
		KEY_TRANS_ENTRY1(F, F, f, F, 'F', F, 2),
		KEY_TRANS_ENTRY1(D, D, d, D, 'D', D, 3),
		KEY_TRANS_ENTRY1(G, G, g, G, 'G', G, 4),
		KEY_TRANS_ENTRY1(2, 2, 2, 2, '2', 2, 5),
		KEY_TRANS_ENTRY1(6, 6, 6, 6, '6', 6, 6),
		KEY_TRANS_ENTRY1(A, A, a, A, 'A', A, 7),
		KEY_TRANS_ENTRY1(S, S, s, S, 'S', S, 8),
		KEY_TRANS_ENTRY1(Q, Q, q, Q, 'Q', Q, 9),
		KEY_TRANS_ENTRY1(W, W, w, W, 'W', W, 10),
		KEY_TRANS_ENTRY1(E, E, e, E, 'E', E, 11),
		KEY_TRANS_ENTRY1(P, P, p, P, 'P', P, 12),
		KEY_TRANS_ENTRY1(O, O, o, O, 'O', O, 13),
		KEY_TRANS_ENTRY1(Y, Y, y, Y, 'Y', Y, 14),

		// player3
		KEY_TRANS_ENTRY1(I, I, i, I, 'I', I, 15),
		KEY_TRANS_ENTRY1(K, K, k, K, 'K', K, 16),
		KEY_TRANS_ENTRY1(J, J, j, J, 'J', J, 17),
		KEY_TRANS_ENTRY1(L, L, l, L, 'L', L, 18),
		KEY_TRANS_ENTRY1(3, 3, 3, 3, '3', 3, 19),
		KEY_TRANS_ENTRY1(7, 7, 7, 7, '7', 7, 20),
		KEY_TRANS_ENTRY1(RCONTROL, RCTRL, RCTRL, RCONTROL, VK_RCONTROL, RightControl, 21),
		KEY_TRANS_ENTRY1(RSHIFT, RSHIFT, RSHIFT, RSHIFT, VK_RSHIFT, RightMenu, 22),
		KEY_TRANS_ENTRY1(ENTER, ENTER, ENTER, ENTER, VK_ENTER, Enter, 23),
		KEY_TRANS_ENTRY1(M, M, m, M, 'M', M, 24),
		KEY_TRANS_ENTRY1(N, N, n, N, 'N', N, 25),
		KEY_TRANS_ENTRY1(B, B, b, B, 'B', B, 26),
		KEY_TRANS_ENTRY1(F1, F1, F1, F1, F1, F1, 27),
		KEY_TRANS_ENTRY1(F2, F2, F2, F2, F2, F2, 28),

		// player4
		KEY_TRANS_ENTRY1(8_PAD, 8_PAD, 8_PAD, 8_PAD, 8_PAD, 8_PAD, 29),
		KEY_TRANS_ENTRY1(2_PAD, 2_PAD, 2_PAD, 2_PAD, 2_PAD, 2_PAD, 30),
		KEY_TRANS_ENTRY1(4_PAD, 4_PAD, 4_PAD, 4_PAD, 4_PAD, 4_PAD, 31),
		KEY_TRANS_ENTRY1(6_PAD, 6_PAD, 6_PAD, 6_PAD, 6_PAD, 6_PAD, 32),
		KEY_TRANS_ENTRY1(4, 4, 4, 4, '4', 4, 42),
		KEY_TRANS_ENTRY1(8, 8, 8, 8, '8', 8, 43),
		KEY_TRANS_ENTRY1(0_PAD, 0_PAD, 0_PAD, 0_PAD, 0_PAD, 0_PAD, 33),
		KEY_TRANS_ENTRY1(DEL_PAD, DEL_PAD, DEL_PAD, DEL_PAD, DEL_PAD, DEL_PAD, 34),
		KEY_TRANS_ENTRY1(ENTER_PAD, ENTER_PAD, ENTER_PAD, ENTER_PAD, ENTER_PAD, ENTER_PAD, 35),
		KEY_TRANS_ENTRY1(F3, F3, F3, F3, F3, F3, 36),
		KEY_TRANS_ENTRY1(F4, F4, F4, F4, F4, F4, 37),
		KEY_TRANS_ENTRY1(F5, F5, F5, F5, F5, F5, 38),
		KEY_TRANS_ENTRY1(F6, F6, F6, F6, F6, F6, 39),
		KEY_TRANS_ENTRY1(F7, F7, F7, F7, F7, F7, 41),

		// player5
		// KEY_TRANS_ENTRY1(F8, F8, F8, F8, F8, F8, 44),
		// KEY_TRANS_ENTRY1(F9, F9, F9, F9, F9, F9, 45),
		// KEY_TRANS_ENTRY1(F10, F10, F10, F10, F10, F10, 46),
		// KEY_TRANS_ENTRY1(F11, F11, F11, F11, F11, F11, 47),
		// KEY_TRANS_ENTRY1(PGUP, PGUP, PGUP, PGUP, PGUP, PGUP, 91),
		// KEY_TRANS_ENTRY1(PGDN, PGDN, PGDN, PGDN, PGDN, PGDN, 92),
		// KEY_TRANS_ENTRY1(F12, F12, F12, F12, F12, F12, 50),
		// KEY_TRANS_ENTRY1(F13, F13, F13, F13, F13, F13, 93),
		// KEY_TRANS_ENTRY1(F14, F14, F14, F14, F14, F14, 94),
		// KEY_TRANS_ENTRY1(F15, F15, F15, F15, F15, F15, 95),
		// KEY_TRANS_ENTRY1(F16, F16, F16, F16, F16, F16, 96),
		// KEY_TRANS_ENTRY1(F17, F17, F17, F17, F17, F17, 97),
		// KEY_TRANS_ENTRY1(F18, F18, F18, F18, F18, F18, 98),
		// KEY_TRANS_ENTRY1(F19, F19, F19, F19, F19, F19, 99),

		KEY_TRANS_ENTRY0(INVALID, UNKNOWN, UNKNOWN, ESCAPE, 0, None, 0, "INVALID")};

	const int NUM_KEYS = sizeof(new_table);

	auto key_trans_entries = std::make_unique<key_trans_entry[]>(NUM_KEYS);
	for (int i = 0; i < NUM_KEYS; i++)
		key_trans_entries[i] = new_table[i];

	// keyboard_trans_table table(std::move(key_trans_entries), (unsigned int)4);
	keyboard_trans_table *table = auto_alloc(machine, keyboard_trans_table(std::move(key_trans_entries), NUM_KEYS));
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
