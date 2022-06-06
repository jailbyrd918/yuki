#include "yuki/core/debug_log.h"
#include "yuki/core/memory/memory_module.h"

#include "yuki/renderer/backend/rasterizer.h"

#include <math.h>
#include <immintrin.h>


bool
rasterizer_clear_buffer
(yuki_render_data *data)
{
	if (!data) {
		YUKI_LOG_ERROR("render data should not be null!");
		return false;
	}

	memory_module_set_block_zero(data->display_buffer, sizeof(color32) * (data->display_width * data->display_height));

	return true;
}

bool
rasterizer_clear_buffer_with_color
(yuki_render_data *data, const color32 color)
{
	if (!data) {
		YUKI_LOG_ERROR("render data should not be null!");
		return false;
	}

	s32	numpx = data->display_width * data->display_height;
	color32	*front = data->display_buffer;
	color32	*back = front + numpx - 1;

	__m256i		_color = _mm256_set1_epi32(color);

	for (; numpx >= 8; front += 8, back -= 8, numpx -= 16) {
		_mm256_storeu_si256((__m256i *)front, _color);
		_mm256_storeu_si256((__m256i *)(back - 7), _color);
	}

	for (; front <= back; *front++ = color, *back-- = color);

	return true;
}


bool
rasterizer_draw_pixel
(yuki_render_data *data, const s32 x, const s32 y, const color32 color)
{
	if (!data) {
		YUKI_LOG_ERROR("render data should not be null!");
		return false;
	}

	data->display_buffer[data->display_width * (y % data->display_height) + (x % data->display_width)] = color;

	return true;
}

bool
rasterizer_draw_line
(yuki_render_data *data, const s32 x1, const s32 y1, const s32 x2, const s32 y2, const color32 color)
{
	if (!data) {
		YUKI_LOG_ERROR("render data should not be null!");
		return false;
	}


	s32	delx = abs(x2 - x1),
		dely = -abs(y2 - y1),
		signx = (x1 < x2) ? 1 : -1,
		signy = (y1 < y2) ? 1 : -1,
		err = delx + dely,
		drawx = x1,
		drawy = y1;

	for (;;) {
		data->display_buffer[data->display_width * (drawy % data->display_height) + (drawx % data->display_width)] = color;

		if (drawx == x2 && drawy == y2)
			break;

		s32 err2 = (err << 1);

		if (err2 >= dely)
			err += dely, drawx += signx;

		if (err2 <= delx)
			err += delx, drawy += signy;
	}

	return true;
}