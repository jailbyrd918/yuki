#ifndef YUKI_HEADER_RENDERER_DEFS
#define YUKI_HEADER_RENDERER_DEFS

#include "yuki/broadcast.h"


typedef	u8		color8;
typedef	u32		color32;
typedef	color32 *	color_buffer;
typedef	f32 *		depth_buffer;

typedef	struct s_yuki_render_data {

	f32		delta_time;
	color_buffer	display_buffer;
	depth_buffer	z_buffer;

	s32		display_width;
	s32		display_height;

}
yuki_render_data;

#define YUKI_ASSIGN_COLOR_RGB(r, g, b)	\
	YUKI_CAST(color32, ((0xff << 24) & 0xff000000) | ((r << 16) & 0x00ff0000) | ((g << 8) & 0x0000ff00) | ((b << 0) & 0x000000ff))

#define YUKI_ASSIGN_COLOR_RGBA(r, g, b, a)	\
	YUKI_CAST(color32, ((a << 24) & 0xff000000) | ((r << 16) & 0x00ff0000) | ((g << 8) & 0x0000ff00) | ((b << 0) & 0x000000ff))


#endif
