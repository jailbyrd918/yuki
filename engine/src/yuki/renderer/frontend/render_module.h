#ifndef YUKI_HEADER_RENDER_MODULE
#define YUKI_HEADER_RENDER_MODULE

#include "yuki/renderer/renderer_defs.h"


bool
render_module_startup
(u64 *required_memory_size, void *state, yuki_window_module *window_module);

void
render_module_shutdown
(void *state);


bool
render_module_draw_frame
(yuki_render_data *data, yuki_window_module *window_module);


#endif
