// license:BSD-3-Clause
// copyright-holders:Olivier Galibert, R. Belmont
#ifndef MAME_OSD_HEADLESS_H
#define MAME_OSD_HEADLESS_H

#pragma once

#include "modules/lib/osdobj_common.h"

extern int osd_num_processors;

class render_target;

typedef struct {
    int width;
    int height;
    uint8_t *buffer;
} image_frame_buf_info_t;

typedef struct {
	const int16_t *buffer;
	const int sample_rate;
	const int samples;
	const int channels;
} sound_frame_buf_info_t;

typedef void (*image_frame_callback_t)(bool);
typedef void (*sound_frame_callback_t)(sound_frame_buf_info_t);

class headless_osd_interface : public osd_common_t
{
public:
	// construction/destruction
	headless_osd_interface(osd_options &options);
	virtual ~headless_osd_interface();

	// general overridables
	virtual void init(running_machine &machine) override;
	virtual void osd_exit() override;

	// virtual void customize_input_type_list(std::vector<input_type_entry> &typelist) override;

	virtual void update(bool skip_redraw) override;
	virtual void input_update() override;

	virtual void video_register() override;
	virtual bool video_init() override;
	virtual void video_exit() override;

	virtual bool window_init() override;
	virtual void window_exit() override;

	virtual void update_audio_stream(const int16_t *buffer, int samples_this_frame) override;

	input_module* input() { return m_keyboard_input; }

	void set_image_buffer_info(image_frame_buf_info_t *buf_info) { m_buffer_info = buf_info; }
	void set_image_frame_cb(image_frame_callback_t fp) { m_image_frame_cb = fp; }
	void set_sound_frame_cb(sound_frame_callback_t fp) { m_sound_frame_cb = fp; }
	void on_machine_setup();

private:
	render_target *m_target;

	image_frame_buf_info_t *m_buffer_info;
	image_frame_callback_t m_image_frame_cb;
	sound_frame_callback_t m_sound_frame_cb;

	void set_starting_view(int index);
	void render_pixels_and_callback(bool skip_redraw);
	void render_pixels_on_buffer();
};

#endif // MAME_OSD_HEADLESS_H
