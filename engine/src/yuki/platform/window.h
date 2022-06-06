#ifndef YUKI_HEADER_WINDOW
#define YUKI_HEADER_WINDOW

#include "yuki/broadcast.h"


typedef struct s_yuki_window_module {

	void	*state;

}
yuki_window_module;


YUKI_API bool
window_module_startup
(yuki_window_module *window_module, const_str title, const s32 x, const s32 y, const s32 width, const s32 height);

YUKI_API bool
window_module_shutdown
(yuki_window_module *window_module);


void
window_module_process_events
(void);


inline YUKI_API const s32
window_module_get_width
(yuki_window_module *window_module);

inline YUKI_API const s32
window_module_get_height
(yuki_window_module *window_module);

inline YUKI_API const struct SDL_Window *
window_module_get_sdl_window
(yuki_window_module *window_module);



#endif
