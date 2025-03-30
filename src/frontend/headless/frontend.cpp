// license:BSD-3-Clause
// copyright-holders:Sangwon Lee
/***************************************************************************

    frontend.cpp

    Command-line interface frontend for MAME.

***************************************************************************/

#include "frontend.h"
#include "manager.h"

//**************************************************************************
//  FRONTEND
//**************************************************************************

//-------------------------------------------------
//  frontend - constructor
//-------------------------------------------------

headless_frontend::headless_frontend(emu_options &options, osd_interface &osd)
	: m_options(options)
	, m_osd(osd)
{
}

//-------------------------------------------------
//  ~frontend - destructor
//-------------------------------------------------

headless_frontend::~headless_frontend()
{
}

//-------------------------------------------------
//  execute - execute a game via the standard
//  command line interface
//-------------------------------------------------

int headless_frontend::execute(std::vector<std::string> &args)
{
	headless_machine_manager manager(m_options, m_osd);

	return manager.execute();
}
