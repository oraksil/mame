// license:BSD-3-Clause
// copyright-holders:Olivier Galibert, R. Belmont
//============================================================
//
//  sdlmain.c - main file for SDLMAME.
//
//  SDLMAME by Olivier Galibert and R. Belmont
//
//============================================================

#include "emu.h"
#include "osdepend.h"
#include "headless_osd.h"

void on_mame_machine_setup();

//============================================================
//  constructor
//============================================================

headless_osd_interface::headless_osd_interface(osd_options &options)
	: osd_common_t(options)
	, m_image_frame_cb(nullptr)
	, m_sound_frame_cb(nullptr)
{
}


//============================================================
//  destructor
//============================================================

headless_osd_interface::~headless_osd_interface()
{
}


//============================================================
//  osd_exit
//============================================================

void headless_osd_interface::osd_exit()
{
	osd_common_t::osd_exit();
}

//============================================================
//  video_register
//============================================================

void headless_osd_interface::video_register()
{
}

//============================================================
//  init
//============================================================

void headless_osd_interface::init(running_machine &machine)
{
	osd_num_processors = 1;

	osd_common_t::init(machine);

	osd_common_t::init_subsystems();
}

void headless_osd_interface::on_machine_setup()
{
	on_mame_machine_setup();
}

