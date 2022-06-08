#include "yuki/core/debug_log.h"
#include "yuki/core/memory/memory_module.h"
#include "yuki/renderer/frontend/texture.h"

#include "yuki/renderer/backend/rasterizer.h"

#include <math.h>
#include <immintrin.h>


static bool
_ykstatic_rasterizer_draw_scanline
(yuki_render_data *data, const s32 x1, const s32 x2, const s32 y, const color32 color)
{
	s32	startx = x1,
		endx = x2,
		drawy = y;

	if (startx > endx)
		YUKI_SWAP(s32, startx, endx);

	bool outofbound = (startx > data->display_width) || (endx < 0) || (drawy < 0) || (drawy > data->display_height);
	if (outofbound)
		return false;
	else {
		startx = YUKI_MATH_CLAMP(startx, 0, data->display_width - 1);
		endx = YUKI_MATH_CLAMP(endx, 0, data->display_width - 1);
		drawy %= data->display_height;

		s32	drawlen = endx - startx;
		color32	*front = data->display_buffer + (data->display_width * drawy) + startx,
			*back = front + drawlen;

		__m256i	_color = _mm256_set1_epi32(color);

		for (; drawlen >= 16; startx += 8, endx -= 8, drawlen -= 16) {
			_mm256_storeu_si256((__m256i *)front, _color);
			_mm256_storeu_si256((__m256i *)(back - 7), _color);
		}

		for (; front <= back; *front++ = color, *back-- = color);
		
		return true;
	}
}

static void
_ykstatic_rasterizer_fill_triangle_flat_side
(yuki_render_data *data, const s32 x1, const s32 y1, const s32 x2, const s32 y2, const s32 x3, const s32 y3, const color32 color)
{
	s32	ldx = abs(x2 - x1),
		ldy = abs(y2 - y1),
		rdx = abs(x3 - x1),
		rdy = abs(y3 - y1),
		lsignx = (x1 < x2) ? 1 : -1,
		lsigny = (y1 < y2) ? 1 : -1,
		rsignx = (x1 < x3) ? 1 : -1,
		rsigny = (y1 < y3) ? 1 : -1;

	bool	lchanged = false,
		rchanged = false;

	if (ldy > ldx) {
		YUKI_SWAP(s32, ldx, ldy);
		lchanged = true;
	}

	if (rdy > rdx) {
		YUKI_SWAP(s32, rdx, rdy);
		rchanged = true;
	}

	s32	lerr = (ldy << 1) - ldx,
		rerr = (rdy << 1) - rdx;

	s32	startx = x1, endx = x1,
		starty = y1, endy = y1;

	for (s32 i = 0; i <= ldx; ++i) {
		_ykstatic_rasterizer_draw_scanline(data, startx, endx, starty, color);

		while (lerr >= 0) {
			if (lchanged)	startx += lsignx;
			else		starty += lsigny;
			lerr -= (ldx << 1);
		}

		if (lchanged)	starty += lsigny;
		else		startx += lsignx;
		lerr += (ldy << 1);

		while (endy != starty) {
			while (rerr >= 0) {
				if (rchanged)	endx += rsignx;
				else		endy += rsigny;
				rerr -= (rdx << 1);
			}

			if (rchanged)	endy += rsigny;
			else		endx += rsignx;
			rerr += (rdy << 1);
		}
	}
}


bool
rasterizer_clear_buffer
(yuki_render_data *data)
{
	if (!data) {
		YUKI_LOG_ERROR("render data is null!");
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
		YUKI_LOG_ERROR("render data is null!");
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
		YUKI_LOG_ERROR("render data is null!");
		return false;
	}

	if (x >= 0 && x < data->display_width && y >= 0 && y < data->display_height)
		data->display_buffer[data->display_width * y + x] = color;

	return true;
}

bool
rasterizer_draw_line
(yuki_render_data *data, const s32 x1, const s32 y1, const s32 x2, const s32 y2, const color32 color)
{
	if (!data) {
		YUKI_LOG_ERROR("render data is null!");
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
		rasterizer_draw_pixel(data, drawx, drawy, color);

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

bool
rasterizer_draw_rect
(yuki_render_data *data, const s32 x, const s32 y, const s32 width, const s32 height, const color32 color)
{
	if (!data) {
		YUKI_LOG_ERROR("render data is null!");
		return false;
	}

	rasterizer_draw_line(data, x, y, x, y + height, color);
	rasterizer_draw_line(data, x + width, y, x + width, y + height, color);
	_ykstatic_rasterizer_draw_scanline(data, x, x + width, y, color);
	_ykstatic_rasterizer_draw_scanline(data, x, x + width, y + height, color);

	return true;
}

bool
rasterizer_fill_rect
(yuki_render_data *data, const s32 x, const s32 y, const s32 width, const s32 height, const color32 color)
{
	if (!data) {
		YUKI_LOG_ERROR("render data is null!");
		return false;
	}

	for (s32 drawy = y; drawy < y + height; ++drawy)
		_ykstatic_rasterizer_draw_scanline(data, x, x + width, drawy, color);

	return true;
}

bool
rasterizer_draw_triangle
(yuki_render_data *data, const s32 x1, const s32 y1, const s32 x2, const s32 y2, const s32 x3, const s32 y3, const color32 color)
{
	if (!data) {
		YUKI_LOG_ERROR("render data is null!");
		return false;
	}

	rasterizer_draw_line(data, x1, y1, x2, y2, color);
	rasterizer_draw_line(data, x1, y1, x3, y3, color);
	rasterizer_draw_line(data, x2, y2, x3, y3, color);

	return true;
}

bool
rasterizer_fill_triangle_solid
(yuki_render_data *data, const s32 x1, const s32 y1, const s32 x2, const s32 y2, const s32 x3, const s32 y3, const color32 color)
{
	if (!data) {
		YUKI_LOG_ERROR("render data is null!");
		return false;
	}

	s32	xa = YUKI_MATH_CLAMP(x1, 0, data->display_width - 1),
		ya = YUKI_MATH_CLAMP(y1, 0, data->display_height - 1),
		xb = YUKI_MATH_CLAMP(x2, 0, data->display_width - 1),
		yb = YUKI_MATH_CLAMP(y2, 0, data->display_height - 1),
		xc = YUKI_MATH_CLAMP(x3, 0, data->display_width - 1),
		yc = YUKI_MATH_CLAMP(y3, 0, data->display_height - 1);

	// sort points by y value
	{
		if (ya > yb) {
			YUKI_SWAP(s32, xa, xb);
			YUKI_SWAP(s32, ya, yb);
		}

		if (yb > yc) {
			YUKI_SWAP(s32, xb, xc);
			YUKI_SWAP(s32, yb, yc);
		}

		if (ya > yb) {
			YUKI_SWAP(s32, xa, xb);
			YUKI_SWAP(s32, ya, yb);
		}
	}

	if (yb == yc)
		_ykstatic_rasterizer_fill_triangle_flat_side(data, xa, ya, xb, yb, xc, yc, color);

	else if (ya == yb)
		_ykstatic_rasterizer_fill_triangle_flat_side(data, xc, yc, xa, ya, xb, yb, color);

	else {
		s32	x4 = YUKI_MATH_CLAMP(YUKI_CAST(s32, ((yb - ya) / YUKI_CAST(f32, yc - ya)) * (xc - xa) + xa), 0, data->display_width - 1),
			y4 = yb;
		
		_ykstatic_rasterizer_fill_triangle_flat_side(data, xa, ya, xb, yb, x4, y4, color);
		_ykstatic_rasterizer_fill_triangle_flat_side(data, xc, yc, xb, yb, x4, y4, color);
	}


	return true;
}

bool
rasterizer_draw_texture_flat
(yuki_render_data *data, const s32 x, const s32 y, const yuki_texture *texture)
{
	if (!data || !texture) {
		YUKI_LOG_ERROR("%s is null!", !data ? "render data" : "reference to texture");
		return false;
	}

	u32	copyw = YUKI_MATH_MIN(YUKI_CAST(u32, data->display_width - x), texture->width),
		copyh = YUKI_MATH_MIN(YUKI_CAST(u32, data->display_height - y), texture->height);

	for (u32 row = 0; row < copyh; ++row) {
		color32	*dprowstart = data->display_buffer + (data->display_width * (y + row)) + x,
			*texrowstart = YUKI_CAST(color_buffer, texture->data) + (texture->width * row);
		memory_module_copy_block(dprowstart, texrowstart, sizeof(color32) * copyw);
	}

	return true;
}


