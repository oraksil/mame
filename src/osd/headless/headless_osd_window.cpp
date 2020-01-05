// license:BSD-3-Clause
// copyright-holders:Sangwon Lee
//============================================================
//
//  window.c - SDL window handling
//
//  SDLMAME by Olivier Galibert and R. Belmont
//
//============================================================

#include "emu.h"
#include "render.h"
#include "headless_osd.h"

bool headless_osd_interface::window_init()
{
	set_starting_view(0);

	return true;
}

void headless_osd_interface::window_exit()
{
}

void headless_osd_interface::set_starting_view(int index)
{
	// query the video system to help us pick a view
	// int viewindex = m_target->configured_view(options().view(index), index, 1);
	int viewindex = 0;

	// set the view
	m_target->set_view(viewindex);
}


