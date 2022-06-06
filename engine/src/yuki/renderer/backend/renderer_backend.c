#include "yuki/core/debug_log.h"
#include "yuki/core/memory/memory_module.h"
#include "yuki/platform/window.h"
#include "yuki/renderer/backend/rasterizer.h"

#include "yuki/renderer/backend/renderer_backend.h"

#include <SDL2/SDL.h>


typedef struct s_yuki_window_module_state {

	SDL_Window	*window;

}
yuki_window_module_state;


static bool
_ykimpl_renderer_backend_startup
(yuki_renderer_backend *backend, yuki_window_module *window_module)
{
	if (!backend) {
		YUKI_LOG_ERROR("render backend instance is null!");
		return false;
	}

	if (!window_module) {
		YUKI_LOG_ERROR("window state is null!");
		return false;
	}

	yuki_window_module_state *state = YUKI_CAST(yuki_window_module_state *, window_module->state);
	backend->display_surface = SDL_GetWindowSurface(state->window);

	return true;
}

static bool
_ykimpl_renderer_backend_shutdown
(yuki_renderer_backend *backend)
{
	if (!backend) {
		YUKI_LOG_ERROR("render backend instance is null!");
		return false;
	}

	return true;
}

static bool
_ykimpl_renderer_backend_begin_frame
(yuki_renderer_backend *backend, yuki_render_data *data)
{
	rasterizer_clear_buffer(data);
	return true;
}

static bool
_ykimpl_renderer_backend_end_frame
(yuki_renderer_backend *backend, yuki_render_data *data, yuki_window_module *window_module)
{
	if (!backend) {
		YUKI_LOG_ERROR("render backend instance is null!");
		return false;
	}
	else if (!data) {
		YUKI_LOG_ERROR("render data instance is null!");
		return false;
	}
	else if (!window_module) {
		YUKI_LOG_ERROR("window state is null!");
		return false;
	}

	SDL_LockSurface(backend->display_surface);

	memory_module_copy_block(
		backend->display_surface->pixels,
		data->display_buffer,
		backend->display_surface->pitch * backend->display_surface->h
	);

	SDL_UnlockSurface(backend->display_surface);

	yuki_window_module_state *state = YUKI_CAST(yuki_window_module_state *, window_module->state);
	SDL_UpdateWindowSurface(state->window);
	return true;
}


bool
renderer_backend_startup
(yuki_renderer_backend *backend, yuki_window_module *window_module)
{
	backend->window_module = window_module;
	backend->pfn_startup = _ykimpl_renderer_backend_startup;
	backend->pfn_shutdown = _ykimpl_renderer_backend_shutdown;
	backend->pfn_begin_frame = _ykimpl_renderer_backend_begin_frame;
	backend->pfn_end_frame = _ykimpl_renderer_backend_end_frame;

	YUKI_LOG_INFO("backend renderer instance is successfully initialized.");
	return true;
}

bool
renderer_backend_shutdown
(yuki_renderer_backend *backend)
{
	if (!backend) {
		YUKI_LOG_ERROR("render backend instance is null!");
		return false;
	}

	backend->window_module = NULL;
	backend->pfn_startup = NULL;
	backend->pfn_shutdown = NULL;
	backend->pfn_begin_frame = NULL;
	backend->pfn_end_frame = NULL;

	return true;
}

