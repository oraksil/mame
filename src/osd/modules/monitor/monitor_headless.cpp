// license:BSD-3-Clause
// copyright-holders: Sangwon Lee
/*
 * monitor_headless.cpp
 *
 */

#include "emu.h"
#include "modules/osdmodule.h"
#include "monitor_module.h"
#include "monitor_common.h"

 //============================================================
 //  headless_monitor_info
 //============================================================

class headless_monitor_info : public osd_monitor_info
{
public:
	headless_monitor_info(
		monitor_module &module,
		std::uint64_t handle,
		const char *monitor_device,
		float aspect)
		: osd_monitor_info(module, handle, monitor_device, aspect)
	{
		headless_monitor_info::refresh();
	}

private:
	void refresh() override {}
};

//============================================================
//  headless_monitor_module
//============================================================

class headless_monitor_module : public monitor_module_base
{
public:
	headless_monitor_module()
		: monitor_module_base(OSD_MONITOR_PROVIDER, "headless") {}

	std::shared_ptr<osd_monitor_info> monitor_from_rect(const osd_rect& proposed) override
	{
		if (!m_initialized)
			return nullptr;

		return list().front();
	}

	std::shared_ptr<osd_monitor_info> monitor_from_window(const osd_window& window) override
	{
		if (!m_initialized)
			return nullptr;

		return list().front();
	}

protected:
	int init_internal(const osd_options& options) override
	{
		// allocate a new monitor info
		std::shared_ptr<headless_monitor_info> monitor =
			std::make_shared<headless_monitor_info>(*this, 0, options.screen(), 1.0f);

		// guess the aspect ratio assuming square pixels
		monitor->set_aspect(static_cast<float>(monitor->position_size().width()) / static_cast<float>(monitor->position_size().height()));

		// hook us into the list
		add_monitor(monitor);

		return 0;
	}
};

MODULE_DEFINITION(MONITOR_HEADLESS, headless_monitor_module)
