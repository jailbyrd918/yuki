#include "yuki/core/debug_log.h"
#include "yuki/core/memory/memory_module.h"
#include "yuki/platform/window.h"
#include "yuki/renderer/backend/rasterizer.h"

#include "yuki/renderer/backend/renderer.h"

#include <SDL2/SDL.h>


typedef struct s_yuki_window_module_state {

	SDL_Window	*window;

}
yuki_window_module_state;


static bool
_ykimpl_renderer_startup
(yuki_renderer *renderer, yuki_window_module *window_module)
{
	if (!renderer) {
		YUKI_LOG_ERROR("render renderer instance is null!");
		return false;
	}

	if (!window_module) {
		YUKI_LOG_ERROR("window state is null!");
		return false;
	}

	yuki_window_module_state *state = YUKI_CAST(yuki_window_module_state *, window_module->state);
	renderer->display_surface = SDL_GetWindowSurface(state->window);

	return true;
}

static bool
_ykimpl_renderer_shutdown
(yuki_renderer *renderer)
{
	if (!renderer) {
		YUKI_LOG_ERROR("render renderer instance is null!");
		return false;
	}

	return true;
}

static bool
_ykimpl_renderer_begin_frame
(yuki_renderer *renderer, yuki_render_data *data)
{
	rasterizer_clear_buffer(data);
	return true;
}

static bool
_ykimpl_renderer_end_frame
(yuki_renderer *renderer, yuki_render_data *data, yuki_window_module *window_module)
{
	if (!renderer) {
		YUKI_LOG_ERROR("render renderer instance is null!");
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

	SDL_LockSurface(renderer->display_surface);

	memory_module_copy_block(
		renderer->display_surface->pixels,
		data->display_buffer,
		renderer->display_surface->pitch * renderer->display_surface->h
	);

	SDL_UnlockSurface(renderer->display_surface);

	yuki_window_module_state *state = YUKI_CAST(yuki_window_module_state *, window_module->state);
	SDL_UpdateWindowSurface(state->window);
	return true;
}


bool
renderer_startup
(yuki_renderer *renderer, yuki_window_module *window_module)
{
	renderer->window_module = window_module;
	renderer->pfn_startup = _ykimpl_renderer_startup;
	renderer->pfn_shutdown = _ykimpl_renderer_shutdown;
	renderer->pfn_begin_frame = _ykimpl_renderer_begin_frame;
	renderer->pfn_end_frame = _ykimpl_renderer_end_frame;

	YUKI_LOG_INFO("renderer is successfully initialized.");
	return true;
}

bool
renderer_shutdown
(yuki_renderer *renderer)
{
	if (!renderer) {
		YUKI_LOG_ERROR("render renderer instance is null!");
		return false;
	}

	renderer->window_module = NULL;
	renderer->pfn_startup = NULL;
	renderer->pfn_shutdown = NULL;
	renderer->pfn_begin_frame = NULL;
	renderer->pfn_end_frame = NULL;

	return true;
}

