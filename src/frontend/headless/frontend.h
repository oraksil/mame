// license:BSD-3-Clause
// copyright-holders:Aaron Giles
/***************************************************************************

    clifront.h

    Command-line interface frontend for MAME.

***************************************************************************/
#ifndef MAME_FRONTEND_HEADLESS_H
#define MAME_FRONTEND_HEADLESS_H

#pragma once

// don't include osd_interface in header files
class emu_options;
class osd_interface;

//**************************************************************************
//  TYPE DEFINITIONS
//**************************************************************************


class headless_frontend
{
public:
	// construction/destruction
	headless_frontend(emu_options &options, osd_interface &osd);
	~headless_frontend();

	// execute based on the incoming argc/argv
	int execute(std::vector<std::string> &args);

private:
	// internal state
	emu_options &m_options;
	osd_interface &m_osd;
};

#endif  // MAME_FRONTEND_HEADLESS_H
