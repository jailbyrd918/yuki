#include "yuki/core/debug_log.h"
#include "yuki/core/memory/memory_module.h"
#include "yuki/platform/window.h"
#include "yuki/gameplay/input.h"
#include "yuki/renderer/backend/rasterizer.h"
#include "yuki/renderer/backend/renderer.h"
#include "yuki/renderer/frontend/texture.h"

#include "yuki/renderer/frontend/render_module.h"

#include <stdlib.h>


typedef struct s_yuki_renderer_module_state {

	yuki_renderer	renderer;

}
yuki_renderer_module_state;

static yuki_renderer_module_state *state_ref;


static bool
_ykstatic_renderer_begin_frame
(yuki_render_data *data)
{
	if (!state_ref) {
		YUKI_LOG_ERROR("renderer subsystem state is null!");
		return false;
	}

	return state_ref->renderer.pfn_begin_frame(&state_ref->renderer, data);
}

static bool
_ykstatic_renderer_end_frame
(yuki_render_data *data, yuki_window_module *window_module)
{
	if (!state_ref) {
		YUKI_LOG_ERROR("renderer subsystem state is null!");
		return false;
	}

	return state_ref->renderer.pfn_end_frame(&state_ref->renderer, data, window_module);
}


bool
render_module_startup
(u64 *required_memory_size, void *state, yuki_window_module *window_module)
{
	*required_memory_size = sizeof(yuki_renderer_module_state);

	if (!state)
		return false;

	state_ref = YUKI_CAST(yuki_renderer_module_state *, state);

	renderer_startup(&state_ref->renderer, window_module);
	state_ref->renderer.frame_count = 0;

	if (!state_ref->renderer.pfn_startup(&state_ref->renderer, window_module)) {
		YUKI_LOG_CRITICAL("renderer renderer failed to initialize!");
		return false;
	}

	return true;
}

void
render_module_shutdown
(void *state)
{
	if (state) {
		state_ref->renderer.pfn_shutdown(&state_ref->renderer);
		state_ref = NULL;
	}
}


bool
render_module_draw_frame
(yuki_render_data *data, yuki_window_module *window_module)
{
	if (_ykstatic_renderer_begin_frame(data)) {

		// start drawing below here ...


		// stop drawing above here ...

		if (!_ykstatic_renderer_end_frame(data, window_module)) {
			YUKI_LOG_CRITICAL("renderer_end_frame failed!");
			return false;
		}
	}

	return true;
}



