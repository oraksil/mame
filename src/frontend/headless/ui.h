// license:BSD-3-Clause
// copyright-holders:Sangwon Lee
/***************************************************************************

    ui.h

    Functions used to handle MAME's crude user interface.

***************************************************************************/

#ifndef MAME_FRONTEND_HEADLESS_UI_H
#define MAME_FRONTEND_HEADLESS_UI_H

#pragma once

#include "ui/uimain.h"

class running_machine;

class headless_ui_manager : public ui_manager
{
public:
	headless_ui_manager(running_machine &machine);
	~headless_ui_manager();
};

#endif  // MAME_FRONTEND_HEADLESS_UI_H
