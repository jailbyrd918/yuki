#include "yuki/core/debug_log.h"
#include "yuki/core/memory/memory_module.h"
#include "yuki/gameplay/event.h"

#include "yuki/gameplay/input.h"

#include <stdlib.h>


typedef struct s_yuki_keyboard_state {

	bool	keys[YUKI_KEYCODES_NUM];

}
yuki_keyboard_state;

typedef struct s_yuki_mouse_state {

	s32	x, y;
	s8	scroll_val;
	bool	buttons[YUKI_MOUSECODES_NUM];

}
yuki_mouse_state;

typedef struct s_yuki_input_module_state {

	yuki_keyboard_state	curr_keyboard_state;
	yuki_keyboard_state	prev_keyboard_state;

	yuki_mouse_state	curr_mouse_state;
	yuki_mouse_state	prev_mouse_state;

}
yuki_input_module_state;

static yuki_input_module_state *state_ref;


bool
input_module_startup
(u64 *required_memory_size, void *state)
{
	*required_memory_size = sizeof(yuki_input_module_state);

	if (!state)
		return false;

	state_ref = YUKI_CAST(yuki_input_module_state *, state);
	memory_module_set_block_zero(state_ref, sizeof(yuki_input_module_state));

	YUKI_LOG_INFO("initialized input subsytem.");
	return true;
}

void
input_module_shutdown
(void *state)
{
	if (state) {
		state_ref = YUKI_CAST(yuki_input_module_state *, state);
		state_ref = NULL;
	}

}

bool
input_module_update
(const f32 delta_time)
{
	if (!state_ref) {
		YUKI_LOG_ERROR("input subsystem state is null!");
		return false;
	}

	// copy current states to previous states
	memory_module_copy_block(&state_ref->prev_keyboard_state, &state_ref->curr_keyboard_state, sizeof(yuki_keyboard_state));
	memory_module_copy_block(&state_ref->prev_mouse_state, &state_ref->curr_mouse_state, sizeof(yuki_mouse_state));

	return true;
}


// keyboard ///////////////////////////////////////////////////////

inline YUKI_API bool
input_module_is_key_down
(const yuki_keycode key)
{
	return state_ref->curr_keyboard_state.keys[key];
}

inline YUKI_API bool
input_module_is_key_up
(const yuki_keycode key)
{
	return !state_ref->curr_keyboard_state.keys[key];
}

inline YUKI_API bool
input_module_was_key_down
(const yuki_keycode key)
{
	return state_ref->prev_keyboard_state.keys[key];
}

inline YUKI_API bool
input_module_was_key_up
(const yuki_keycode key)
{
	return !state_ref->prev_keyboard_state.keys[key];
}

inline YUKI_API bool
input_module_is_key_pressed
(const yuki_keycode key)
{
	return (input_module_is_key_up(key) && input_module_was_key_down(key));
}


void
input_module_process_key_input
(const yuki_keycode key, const bool pressed)
{
	if (state_ref && state_ref->curr_keyboard_state.keys[key] != pressed) {
		// update keyboard internal state
		state_ref->curr_keyboard_state.keys[key] = pressed;

		// fire off the event for immediate processing
		{
			yuki_event_context ctx;
			ctx.data.u16[0] = key;
			event_module_fire_event(NULL, pressed ? YUKI_EVENT_CODE_KEY_PRESSED : YUKI_EVENT_CODE_KEY_RELEASED, ctx);
		}
	}
}


// mouse //////////////////////////////////////////////////////////

inline YUKI_API bool
input_module_is_mouse_button_down
(const yuki_mousecode mouse_btn)
{
	return state_ref->curr_mouse_state.buttons[mouse_btn];
}

inline YUKI_API bool
input_module_is_mouse_button_up
(const yuki_mousecode mouse_btn)
{
	return state_ref->prev_mouse_state.buttons[mouse_btn];
}

inline YUKI_API bool
input_module_was_mouse_button_down
(const yuki_mousecode mouse_btn)
{
	return !state_ref->curr_mouse_state.buttons[mouse_btn];
}

inline YUKI_API bool
input_module_was_mouse_button_up
(const yuki_mousecode mouse_btn)
{
	return !state_ref->prev_mouse_state.buttons[mouse_btn];
}


inline YUKI_API const s32
input_module_get_mouse_x
(void)
{
	return state_ref->curr_mouse_state.x;
}

inline YUKI_API const s32
input_module_get_mouse_y
(void)
{
	return state_ref->curr_mouse_state.y;
}

inline YUKI_API const s32
input_module_get_prev_mouse_x
(void)
{
	return state_ref->prev_mouse_state.x;
}

inline YUKI_API const s32
input_module_get_prev_mouse_y
(void)
{
	return state_ref->prev_mouse_state.y;
}

inline YUKI_API const s32
input_module_get_mouse_scroll_value
(void)
{
	return state_ref->curr_mouse_state.scroll_val;
}

inline YUKI_API const s32
input_module_get_prev_mouse_scroll_value
(void)
{
	return state_ref->prev_mouse_state.scroll_val;
}


void
input_module_process_mouse_button
(const yuki_mousecode mouse_button, const bool pressed)
{
	if (state_ref->curr_mouse_state.buttons[mouse_button] != pressed) {
		state_ref->curr_mouse_state.buttons[mouse_button] = pressed;

		// fire off the event for immediate processing
		{
			yuki_event_context ctx;
			ctx.data.u16[0] = mouse_button;
			event_module_fire_event(NULL, pressed ? YUKI_EVENT_CODE_MOUSE_BTN_PRESSED : YUKI_EVENT_CODE_MOUSE_BTN_RELEASED, ctx);
		}
	}
}

void
input_module_process_mouse_move
(const s32 x, const s32 y)
{
	if ((state_ref->curr_mouse_state.x != x) || (state_ref->curr_mouse_state.y != y)) {
		// update mouse internal state
		state_ref->curr_mouse_state.x = x;
		state_ref->curr_mouse_state.y = y;

		// fire off the event for immediate processing
		{
			yuki_event_context ctx;
			ctx.data.u16[0] = x;
			ctx.data.u16[1] = y;
			event_module_fire_event(NULL, YUKI_EVENT_CODE_MOUSE_MOVED, ctx);
		}
	}
}

void
input_module_process_mouse_scroll
(const s8 scroll_value)
{
	// update mouse internal state
	state_ref->curr_mouse_state.scroll_val = scroll_value;

	// fire off the event for immediate processing
	{
		yuki_event_context ctx;
		ctx.data.u16[0] = scroll_value;
		event_module_fire_event(NULL, YUKI_EVENT_CODE_MOUSE_SCROLLED, ctx);
	}
}



