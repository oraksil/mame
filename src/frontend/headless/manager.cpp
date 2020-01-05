// license:BSD-3-Clause
// copyright-holders:Sangwon Lee
/***************************************************************************

    manager.cpp

    Controls execution of the core MAME system.

***************************************************************************/

#include "manager.h"
#include "emuopts.h"
#include "ui.h"
#include "headless/headless_osd.h"

//-------------------------------------------------
//  headless_machine_manager - constructor
//-------------------------------------------------

headless_machine_manager::headless_machine_manager(emu_options &options, osd_interface &osd) :
	machine_manager(options, osd)
{
}


//-------------------------------------------------
//  ~headless_machine_manager - destructor
//-------------------------------------------------

headless_machine_manager::~headless_machine_manager()
{
}

//-------------------------------------------------
//  execute - run the core emulation
//-------------------------------------------------

int headless_machine_manager::execute()
{
    start_http_server();

    const game_driver *system = m_options.system();

    machine_config config(*system, m_options);

	running_machine machine(config, *this);

	set_machine(&machine);

	return machine.run(false);
}

/***************************************************************************
    CORE IMPLEMENTATION
***************************************************************************/

//-------------------------------------------------
//  update_machine
//-------------------------------------------------

void headless_machine_manager::update_machine()
{
	headless_osd_interface &osd = dynamic_cast<headless_osd_interface&>(m_osd);
    osd.on_machine_setup();
}

ui_manager* headless_machine_manager::create_ui(running_machine& machine)
{
	m_ui = std::make_unique<headless_ui_manager>(machine);

	return m_ui.get();
}

void headless_machine_manager::ui_initialize(running_machine& machine)
{
}

void headless_machine_manager::before_load_settings(running_machine& machine)
{
}

void headless_machine_manager::create_custom(running_machine& machine)
{
}

void headless_machine_manager::load_cheatfiles(running_machine& machine)
{
}