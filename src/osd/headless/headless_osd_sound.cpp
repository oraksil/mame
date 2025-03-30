// license:BSD-3-Clause
// copyright-holders:Olivier Galibert, R. Belmont
//============================================================
//
//  video.c - SDL video handling
//
//  SDLMAME by Olivier Galibert and R. Belmont
//
//============================================================

#include "emu.h"
#include "headless_osd.h"

void headless_osd_interface::update_audio_stream(const int16_t *buffer, int samples_this_frame)
{
	// osd_printf_debug("sound sample, %p, size of %d\n", buffer, bytes_this_frame);
	if (m_sound_frame_cb)
	{
		sound_frame_buf_info_t sound_buf = {
			.buffer = buffer,
			.sample_rate = 48000,
			.samples = samples_this_frame,
			.channels = 2
		};
		(*m_sound_frame_cb)(sound_buf);
	}
}