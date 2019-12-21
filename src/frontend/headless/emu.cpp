// license:BSD-3-Clause
// copyright-holders:Sangwon Lee
/***************************************************************************

    emu.c

    Controls execution of the core MAME system.

***************************************************************************/

#include "emu.h"
#include "frontend.h"

extern const char build_version[];
extern const char bare_build_version[];

const char * emulator_info::get_bare_build_version()
{
	return bare_build_version;
}

const char * emulator_info::get_build_version()
{
	return build_version;
}

void emulator_info::display_ui_chooser(running_machine& machine)
{
}

int emulator_info::start_frontend(emu_options &options, osd_interface &osd, std::vector<std::string> &args)
{
	headless_frontend front(options, osd);

	return front.execute(args);
}

void emulator_info::draw_user_interface(running_machine& machine)
{
}

void emulator_info::periodic_check()
{
}

bool emulator_info::frame_hook()
{
	return false;
}

void emulator_info::sound_hook()
{
}

void emulator_info::layout_file_cb(util::xml::data_node const &layout)
{
}

bool emulator_info::standalone()
{
	return false;
}
