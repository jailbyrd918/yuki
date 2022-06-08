#ifndef YUKI_HEADER_RASTERIZER
#define YUKI_HEADER_RASTERIZER

#include "yuki/math/math_defs.h"
#include "yuki/renderer/renderer_defs.h"
#include "yuki/resources/resource_defs.h"


bool
rasterizer_clear_buffer
(yuki_render_data *data);

bool
rasterizer_clear_buffer_with_color
(yuki_render_data *data, const color32 color);


bool
rasterizer_draw_pixel
(yuki_render_data *data, const s32 x, const s32 y, const color32 color);

bool
rasterizer_draw_line
(yuki_render_data *data, const s32 x1, const s32 y1, const s32 x2, const s32 y2, const color32 color);

bool
rasterizer_draw_rect
(yuki_render_data *data, const s32 x, const s32 y, const s32 width, const s32 height, const color32 color);

bool
rasterizer_fill_rect
(yuki_render_data *data, const s32 x, const s32 y, const s32 width, const s32 height, const color32 color);

bool
rasterizer_draw_triangle
(yuki_render_data *data, const s32 x1, const s32 y1, const s32 x2, const s32 y2, const s32 x3, const s32 y3, const color32 color);

bool
rasterizer_fill_triangle_solid
(yuki_render_data *data, const s32 x1, const s32 y1, const s32 x2, const s32 y2, const s32 x3, const s32 y3, const color32 color);

bool
rasterizer_draw_texture_flat
(yuki_render_data *data, const s32 x, const s32 y, const yuki_texture *texture);


#endif
