#ifndef YUKI_HEADER_INPUT
#define YUKI_HEADER_INPUT

#include "yuki/gameplay/input_defs.h"


bool
input_module_startup
(u64 *required_memory_size, void *state);

void
input_module_shutdown
(void *state);

bool
input_module_update
(const f32 delta_time);


// keyboard ///////////////////////////////////////////////////////

inline YUKI_API bool
input_module_is_key_down
(const yuki_keycode key);

inline YUKI_API bool
input_module_is_key_up
(const yuki_keycode key);

inline YUKI_API bool
input_module_was_key_down
(const yuki_keycode key);

inline YUKI_API bool
input_module_was_key_up
(const yuki_keycode key);

inline YUKI_API bool
input_module_is_key_pressed
(const yuki_keycode key);


void
input_module_process_key_input
(const yuki_keycode key, const bool pressed);


// mouse //////////////////////////////////////////////////////////

inline YUKI_API bool
input_module_is_mouse_button_down
(const yuki_mousecode mouse_btn);

inline YUKI_API bool
input_module_is_mouse_button_up
(const yuki_mousecode mouse_btn);

inline YUKI_API bool
input_module_was_mouse_button_down
(const yuki_mousecode mouse_btn);

inline YUKI_API bool
input_module_was_mouse_button_up
(const yuki_mousecode mouse_btn);

inline YUKI_API const s32
input_module_get_mouse_x
(void);

inline YUKI_API const s32
input_module_get_mouse_y
(void);

inline YUKI_API const s32
input_module_get_prev_mouse_x
(void);

inline YUKI_API const s32
input_module_get_prev_mouse_y
(void);

inline YUKI_API const s32
input_module_get_mouse_scroll_value
(void);

inline YUKI_API const s32
input_module_get_prev_mouse_scroll_value
(void);


void
input_module_process_mouse_button
(const yuki_mousecode mouse_button, const bool pressed);

void
input_module_process_mouse_move
(const s32 x, const s32 y);

void
input_module_process_mouse_scroll
(const s8 scroll_value);


#endif
