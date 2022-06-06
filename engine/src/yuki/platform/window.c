#include "yuki/core/debug_log.h"
#include "yuki/core/assertions.h"
#include "yuki/core/memory/memory_module.h"
#include "yuki/gameplay/event.h"
#include "yuki/gameplay/input.h"

#include "yuki/platform/window.h"

#include <SDL2/SDL.h>


typedef struct s_yuki_window_module_state {

	SDL_Window	*window;

	s32		x, y;
	s32		width, height;

}
yuki_window_module_state;


YUKI_API bool
window_module_startup
(yuki_window_module *window_module, const_str title, const s32 x, const s32 y, const s32 width, const s32 height)
{
	window_module->state = malloc(sizeof(yuki_window_module_state));

	yuki_window_module_state *state = YUKI_CAST(yuki_window_module_state *, window_module->state);

	// create SDL window
	state->window = SDL_CreateWindow(title, x, y, width, height, SDL_WINDOW_SHOWN);
	YUKI_ASSERT_MSG(state->window, "failed to create SDL window!");

	// initialize window state properties
	SDL_GetWindowPosition(state->window, &state->x, &state->y);
	SDL_GetWindowSize(state->window, &state->width, &state->height);

	return true;
}

YUKI_API bool
window_module_shutdown
(yuki_window_module *window_module)
{
	if (!window_module) {
		YUKI_LOG_ERROR("window_module is null!");
		return false;
	}

	yuki_window_module_state *state = YUKI_CAST(yuki_window_module_state *, window_module->state);

	// destroy window
	SDL_DestroyWindow(state->window);

	// deallocate window state
	free(window_module->state);

	return true;
}


void
window_module_process_events
(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
			{
				yuki_event_context ctx = { 0 };
				event_module_fire_event(NULL, YUKI_EVENT_CODE_APP_QUIT, ctx);
			}
			break;

			case SDL_KEYDOWN:
			case SDL_KEYUP:
			{
				bool		pressed = (event.type == SDL_KEYDOWN);
				yuki_keycode	key = YUKI_CAST(yuki_keycode, event.key.keysym.scancode);

				input_module_process_key_input(key, pressed);
			}
			break;

			case SDL_MOUSEMOTION:
			{
				s32	mousex = event.motion.x,
					mousey = event.motion.y;

				input_module_process_mouse_move(mousex, mousey);
			}
			break;

			case SDL_MOUSEWHEEL:
			{
				s32	scrollval = event.wheel.y;
				if (scrollval)
					input_module_process_mouse_scroll(scrollval);
			}
			break;

			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			{
				bool		pressed = (event.button.type == SDL_MOUSEBUTTONDOWN);
				yuki_mousecode	mouse_button;

				switch (event.button.button) {
					case SDL_BUTTON_LEFT:
						mouse_button = YUKI_MOUSECODE_LEFT;
						break;

					case SDL_BUTTON_MIDDLE:
						mouse_button = YUKI_MOUSECODE_MIDDLE;
						break;

					case SDL_BUTTON_RIGHT:
						mouse_button = YUKI_MOUSECODE_RIGHT;
						break;
				}

				if (mouse_button != YUKI_MOUSECODES_NUM)
					input_module_process_mouse_button(mouse_button, pressed);
			}
			break;

			default:
				break;
		}
	};
}


inline YUKI_API const s32
window_module_get_width
(yuki_window_module *window_module)
{
	return YUKI_CAST(yuki_window_module_state *, window_module->state)->width;
}

inline YUKI_API const s32
window_module_get_height
(yuki_window_module *window_module)
{
	return YUKI_CAST(yuki_window_module_state *, window_module->state)->height;
}

inline YUKI_API const struct SDL_Window *
window_module_get_sdl_window
(yuki_window_module *window_module)
{
	return YUKI_CAST(yuki_window_module_state *, window_module->state)->window;
}
