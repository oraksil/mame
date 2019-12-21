// license:BSD-3-Clause
// copyright-holders:Sangwon Lee
/*********************************************************************

    ui.cpp

    Functions used to handle MAME's user interface.

*********************************************************************/

#include "ui.h"

headless_ui_manager::headless_ui_manager(running_machine &machine)
	: ui_manager(machine)
{
}

headless_ui_manager::~headless_ui_manager()
{
}
