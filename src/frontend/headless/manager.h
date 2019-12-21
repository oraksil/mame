// license:BSD-3-Clause
// copyright-holders:Sangwon Lee
/***************************************************************************

    manager.h

    Controls execution of the core MAME system.
***************************************************************************/

#ifndef MAME_FRONTEND_HEADLESS_MANAGER_H
#define MAME_FRONTEND_HEADLESS_MANAGER_H

#pragma once

class emu_options;
class osd_interface;
class headless_ui_manager;
class running_machine;

//**************************************************************************
//    TYPE DEFINITIONS
//**************************************************************************

// ======================> machine_manager

class headless_machine_manager : public machine_manager
{
public:
	headless_machine_manager(emu_options &options, osd_interface &osd);
	~headless_machine_manager();

	virtual ui_manager* create_ui(running_machine& machine);
	virtual void create_custom(running_machine& machine);
	virtual void load_cheatfiles(running_machine& machine);
	virtual void ui_initialize(running_machine& machine);
	virtual void before_load_settings(running_machine &machine);
	virtual void update_machine();

	int execute();

private:
	std::unique_ptr<headless_ui_manager> m_ui;
};

#endif // MAME_FRONTEND_HEADLESS_MANAGER_H
