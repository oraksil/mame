// license:BSD-3-Clause
// copyright-holders:Olivier Galibert, R. Belmont
//============================================================
//
//  sdlmain.c - main file for SDLMAME.
//
//  SDLMAME by Olivier Galibert and R. Belmont
//
//============================================================

// standard includes

// MAME headers
#include "emu.h"

// OSD headers
#include "osdepend.h"
#include "osdheadless.h"

//============================================================
//  main
//============================================================

// int osd_entrypoint(int argc, char** argv, image_buffer_info_t *buf_info, update_callback_t cb)
int osd_entrypoint(int argc, char** argv, image_buffer_info_t *buf_info, update_callback_t cb, void** retOSD)
{
	std::vector<std::string> args = osd_get_command_line(argc, argv);
	int res = 0;

	// disable I/O buffering
	setvbuf(stdout, (char *) nullptr, _IONBF, 0);
	setvbuf(stderr, (char *) nullptr, _IONBF, 0);

	osd_options options;
	options.set_system_name("dino");
	options.set_value(OSD_MONITOR_PROVIDER, "headless", OPTION_PRIORITY_MAXIMUM);
	options.set_value(OSD_KEYBOARDINPUT_PROVIDER, "headless", OPTION_PRIORITY_MAXIMUM);
	options.set_value(OSDOPTION_SCREEN, OSDOPTVAL_AUTO, OPTION_PRIORITY_MAXIMUM);
	options.set_value(OSDOPTION_WINDOW, 1, OPTION_PRIORITY_MAXIMUM);
	options.set_value(OSDOPTION_RESOLUTION, "960x720@60", OPTION_PRIORITY_MAXIMUM);

	headless_osd_interface osd(options);
	osd.register_options();
	osd.set_buffer_info(buf_info);
	osd.set_update_callback(cb);

	*retOSD = (void *)&osd;

	res = emulator_info::start_frontend(options, osd, args);

	exit(res);
}

//============================================================
//  constructor
//============================================================

headless_osd_interface::headless_osd_interface(osd_options &options)
	: osd_common_t(options)
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
