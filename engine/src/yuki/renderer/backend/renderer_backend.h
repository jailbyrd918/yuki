#ifndef YUKI_HEADER_RENDERER_BACKEND
#define YUKI_HEADER_RENDERER_BACKEND

#include "yuki/renderer/renderer_defs.h"


typedef struct s_yuki_renderer_backend {

	yuki_window_module	*window_module;
	struct SDL_Surface	*display_surface;
	u64			frame_count;

	bool(*pfn_startup)(struct s_yuki_renderer_backend *backend, yuki_window_module *window_module);
	bool(*pfn_shutdown)(struct s_yuki_renderer_backend *backend);
	bool(*pfn_begin_frame)(struct s_yuki_renderer_backend *backend, yuki_render_data *data);
	bool(*pfn_end_frame)(struct s_yuki_renderer_backend *backend, yuki_render_data *data, yuki_window_module *window_module);

}
yuki_renderer_backend;


bool
renderer_backend_startup
(yuki_renderer_backend *backend, yuki_window_module *window_module);

bool
renderer_backend_shutdown
(yuki_renderer_backend *backend);



#endif

