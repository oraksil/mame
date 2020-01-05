// license:BSD-3-Clause
// copyright-holders:Olivier Galibert, R. Belmont
#ifndef MAME_OSD_HEADLESS_H
#define MAME_OSD_HEADLESS_H

#pragma once

#include "modules/lib/osdobj_common.h"
#include "headless.h"

extern int osd_num_processors;

class render_target;

typedef void (*update_callback_t)(bool);

typedef struct {
    int width;
    int height;
    uint8_t *buffer;
} image_buffer_info_t;

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

	input_module* input() { return m_keyboard_input; }

	void set_update_callback(update_callback_t fp) { m_callback = fp; }
	void set_buffer_info(image_buffer_info_t *buf_info) { m_buffer_info = buf_info; }
	void on_machine_setup();

private:
	render_target *m_target;

	update_callback_t m_callback;
	image_buffer_info_t *m_buffer_info;

	void set_starting_view(int index);
	void render_pixels_and_callback(bool skip_redraw);
	void render_pixels_on_buffer();
};

#endif // MAME_OSD_HEADLESS_H
