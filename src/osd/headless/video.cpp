// license:BSD-3-Clause
// copyright-holders:Olivier Galibert, R. Belmont
//============================================================
//
//  video.c - SDL video handling
//
//  SDLMAME by Olivier Galibert and R. Belmont
//
//============================================================

// MAME headers
#include "emu.h"

// MAMEOS headers
#include "osdheadless.h"

// MODULES headers
#include "rendersw.hxx"


//============================================================
//  CONSTANTS
//============================================================


//============================================================
//  GLOBAL VARIABLES
//============================================================


//============================================================
//  LOCAL VARIABLES
//============================================================


//============================================================
//  video_init
//============================================================

bool headless_osd_interface::video_init()
{
	m_target = machine().render().target_alloc();

	// initialize the window system so we can make windows
	if (!window_init())
	{
		return false;
	}

	return true;
}

//============================================================
//  video_exit
//============================================================

void headless_osd_interface::video_exit()
{
	window_exit();
}

//============================================================
//  update
//============================================================

void headless_osd_interface::update(bool skip_redraw)
{
	osd_common_t::update(skip_redraw);

	render_pixels_and_callback(skip_redraw);
}

void headless_osd_interface::render_pixels_and_callback(bool skip_redraw)
{
	render_pixels_on_buffer();

	if (m_callback)
		(*m_callback)(skip_redraw);
}

void headless_osd_interface::render_pixels_on_buffer()
{
	if (m_buffer_info == nullptr || m_buffer_info->buffer == nullptr)
		return;

	m_target->set_bounds(m_buffer_info->width, m_buffer_info->height, 1.f);

	auto &prim = m_target->get_primitives();
	prim.acquire_lock();
	software_renderer<uint32_t, 0, 0, 0, 16, 8, 0>::draw_primitives(
		prim,
		m_buffer_info->buffer,
		m_buffer_info->width,
		m_buffer_info->height,
		m_buffer_info->width
	);
	prim.release_lock();
}

//============================================================
//  input_update
//============================================================

void headless_osd_interface::input_update()
{
}