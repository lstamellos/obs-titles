/*
obs-text-slideshow
Copyright (C) 2021 Joshua Wong jbwong05@gmail.com

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>
*/

#include <obs-module.h>
#include <obs-frontend-api.h>
#include <QMainWindow>
#include <QWidget>

#include "plugin-macros.generated.h"
#include "obs-text-slideshow-dock.h"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US")

#define TEXT_SLIDESHOW_DOCK_ID "obs-text-slideshow-dock"
#define TEXT_SLIDESHOW_DOCK_TITLE "Text Slideshow"

extern void load_text_freetype2_slideshow();
#ifdef _WIN32
extern void load_text_gdiplus_slideshow();
#endif

bool obs_module_load(void)
{
	load_text_freetype2_slideshow();
#ifdef _WIN32
	load_text_gdiplus_slideshow();
#endif

	const auto main_window =
		static_cast<QMainWindow *>(obs_frontend_get_main_window());
	obs_frontend_push_ui_translation(obs_module_get_string);
	auto *dock = new TextSlideshowDock(main_window);
	const bool dock_added = obs_frontend_add_dock_by_id(
		TEXT_SLIDESHOW_DOCK_ID, TEXT_SLIDESHOW_DOCK_TITLE,
		static_cast<QWidget *>(dock));
	obs_frontend_pop_ui_translation();

	if (!dock_added) {
		blog(LOG_WARNING, "failed to add dock: id already in use");
		delete dock;
	}

	blog(LOG_INFO, "plugin loaded successfully (version %s)",
	     PLUGIN_VERSION);
	return true;
}

void obs_module_unload()
{
	obs_frontend_remove_dock(TEXT_SLIDESHOW_DOCK_ID);
	blog(LOG_INFO, "plugin unloaded");
}
