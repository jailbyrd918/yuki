#ifndef YUKI_HEADER_RENDERER
#define YUKI_HEADER_RENDERER

#include "yuki/renderer/renderer_defs.h"


typedef struct s_yuki_renderer {

	yuki_window_module	*window_module;
	struct SDL_Surface	*display_surface;
	u64			frame_count;

	bool(*pfn_startup)(struct s_yuki_renderer *backend, yuki_window_module *window_module);
	bool(*pfn_shutdown)(struct s_yuki_renderer *backend);
	bool(*pfn_begin_frame)(struct s_yuki_renderer *backend, yuki_render_data *data);
	bool(*pfn_end_frame)(struct s_yuki_renderer *backend, yuki_render_data *data, yuki_window_module *window_module);

}
yuki_renderer;


bool
renderer_startup
(yuki_renderer *backend, yuki_window_module *window_module);

bool
renderer_shutdown
(yuki_renderer *backend);



#endif

