#include "yuki/math/matrix.h"

#include "yuki/math/vector.h"

#include <math.h>
#include <immintrin.h>


// 2d vector ///////////////////////////////////////////////////////////////

const yuki_vec2
vec2_new
(const f32 x, const f32 y)
{
	return (yuki_vec2) { x, y };
}

const yuki_vec2
vec2_copy
(const yuki_vec2 vector)
{
	return (yuki_vec2) { vector.x, vector.y };
}

const yuki_vec2
vec2_zero
(void)
{
	return (yuki_vec2) { 0.0f, 0.0f };
}

const yuki_vec2
vec2_one
(void)
{
	return (yuki_vec2) { 1.0f, 1.0f };;
}

const yuki_vec2
vec2_up
(void)
{
	return (yuki_vec2) { 0.0f, 1.0f };
}

const yuki_vec2
vec2_down
(void)
{
	return (yuki_vec2) { 0.0f, -1.0f };
}

const yuki_vec2
vec2_left
(void)
{
	return (yuki_vec2) { -1.0f, 0.0f };
}

const yuki_vec2
vec2_right
(void)
{
	return (yuki_vec2) { 1.0f, 0.0f };
}

const yuki_vec2
vec2_add
(const yuki_vec2 left, const yuki_vec2 right)
{
	return (yuki_vec2) { left.x + right.x, left.y + right.y };
}

const yuki_vec2
vec2_sub
(const yuki_vec2 left, const yuki_vec2 right)
{
	return (yuki_vec2) { left.x - right.x, left.y - right.y };
}

const yuki_vec2
vec2_mul_scalar
(const yuki_vec2 vector, const f32 scalar)
{
	return (yuki_vec2) { vector.x * scalar, vector.y * scalar };
}

const yuki_vec2
vec2_div_scalar
(const yuki_vec2 vector, const f32 scalar)
{
	return (yuki_vec2) { scalar != 0 ? (vector.x / scalar) : 0.0f, scalar != 0 ? (vector.y / scalar) : 0.0f };
}

const yuki_vec2
vec2_mul_vec2
(const yuki_vec2 left, const yuki_vec2 right)
{
	return (yuki_vec2) { left.x * right.x, left.y * right.y };
}

const yuki_vec2
vec2_div_vec2
(const yuki_vec2 left, const yuki_vec2 right)
{
	return (yuki_vec2)
	{
		right.x != 0 ? (left.x / right.x) : 0.0f, right.y != 0 ? (left.y / right.y) : 0.0f
	};
}

const f32
vec2_get_magnitude
(const yuki_vec2 vector)
{
	return sqrtf((vector.x * vector.x) + (vector.y * vector.y));
}

bool
vec2_normalize
(yuki_vec2 *vector)
{
	if (!vector)
		return false;

	const f32 magnitude = vec2_get_magnitude(*vector);
	if (magnitude != 0) {
		vector->x /= magnitude;
		vector->y /= magnitude;
	}

	return true;
}

const yuki_vec2
vec2_get_normalized
(yuki_vec2 vector)
{
	vec2_normalize(&vector);
	return vector;
}

const f32
vec2_dot_product
(const yuki_vec2 left, const yuki_vec2 right)
{
	return (left.x * right.x) + (right.y * right.y);
}


// 3d vector ///////////////////////////////////////////////////////////////

const yuki_vec3
vec3_new
(const f32 x, const f32 y, const f32 z)
{
	return (yuki_vec3) { x, y, z };
}

const yuki_vec3
vec3_copy
(const yuki_vec3 vector)
{
	return (yuki_vec3) { vector.x, vector.y, vector.z };
}

const yuki_vec3
vec3_zero
(void)
{
	return (yuki_vec3) { 0.0f, 0.0f, 0.0f };
}

const yuki_vec3
vec3_one
(void)
{
	return (yuki_vec3) { 1.0f, 1.0f, 1.0f };
}

const yuki_vec3
vec3_up
(void)
{
	return (yuki_vec3) { 0.0f, 1.0f, 0.0f };
}

const yuki_vec3
vec3_down
(void)
{
	return (yuki_vec3) { 0.0f, -1.0f, 0.0f };
}

const yuki_vec3
vec3_left
(void)
{
	return (yuki_vec3) { -1.0f, 0.0f, 0.0f };
}

const yuki_vec3
vec3_right
(void)
{
	return (yuki_vec3) { 1.0f, 0.0f, 0.0f };
}

const yuki_vec3
vec3_forward
(void)
{
	return (yuki_vec3) { 0.0f, 0.0f, 1.0f };
}

const yuki_vec3
vec3_backward
(void)
{
	return (yuki_vec3) { 0.0f, 0.0f, -1.0f };
}

const yuki_vec3
vec3_add
(const yuki_vec3 left, const yuki_vec3 right)
{
	return (yuki_vec3) { left.x + right.x, left.y + right.y, left.z + right.z };
}

const yuki_vec3
vec3_sub
(const yuki_vec3 left, const yuki_vec3 right)
{
	return (yuki_vec3) { left.x - right.x, left.y - right.y, left.z - right.z };
}

const yuki_vec3
vec3_mul_scalar
(const yuki_vec3 vector, const f32 scalar)
{
	return (yuki_vec3) { vector.x * scalar, vector.y * scalar, vector.z * scalar };
}

const yuki_vec3
vec3_div_scalar
(const yuki_vec3 vector, const f32 scalar)
{
	return (yuki_vec3)
	{
		scalar != 0 ? (vector.x / scalar) : 0.0f, scalar != 0 ? (vector.y / scalar) : 0.0f, scalar != 0 ? (vector.z / scalar) : 0.0f
	};
}

const yuki_vec3
vec3_mul_vec3
(const yuki_vec3 left, const yuki_vec3 right)
{
	return (yuki_vec3) { left.x * right.x, left.y * right.y, left.z * right.z };
}

const yuki_vec3
vec3_div_vec3
(const yuki_vec3 left, const yuki_vec3 right)
{
	return (yuki_vec3)
	{
		right.x != 0 ? (left.x / right.x) : 0.0f, right.y != 0 ? (left.y / right.y) : 0.0f, right.z != 0 ? (left.z / right.z) : 0.0f
	};
}

const f32
vec3_get_magnitude
(const yuki_vec3 vector)
{
	return sqrtf((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
}

bool
vec3_normalize
(yuki_vec3 *vector)
{
	if (!vector)
		return false;

	const f32 magnitude = vec3_get_magnitude(*vector);
	if (magnitude != 0) {
		vector->x /= magnitude;
		vector->y /= magnitude;
		vector->z /= magnitude;
	}

	return true;
}

const yuki_vec3
vec3_get_normalized
(yuki_vec3 vector)
{
	vec3_normalize(&vector);
	return vector;
}

const f32
vec3_dot_product
(const yuki_vec3 left, const yuki_vec3 right)
{
	return (left.x * right.x) + (left.y * right.y) + (left.z * right.z);
}

const yuki_vec3
vec3_cross_product
(const yuki_vec3 left, const yuki_vec3 right)
{
	return (yuki_vec3)
	{
		left.y * right.z - left.z * right.y, left.z * right.x - left.x * right.z, left.x * right.y - left.y * right.x
	};
}



// 4d vector ///////////////////////////////////////////////////////////////

const yuki_vec4
vec4_new
(const f32 x, const f32 y, const f32 z, const f32 w)
{
	return (yuki_vec4) { x, y, z, w };
}

const yuki_vec4
vec4_copy
(const yuki_vec4 vector)
{
	return (yuki_vec4) { vector.x, vector.y, vector.z, vector.w };
}


const yuki_vec4
vec4_zero
(void)
{
	return (yuki_vec4) { 0.0f, 0.0f, 0.0f, 0.0f };
}

const yuki_vec4
vec4_one
(void)
{
	return (yuki_vec4) { 1.0f, 1.0f, 1.0f, 1.0f };
}

const yuki_vec4
vec4_add
(const yuki_vec4 left, const yuki_vec4 right)
{
	__m128	_left = _mm_loadr_ps(left.elements),
		_right = _mm_loadr_ps(right.elements);

	yuki_vec4 result;
	_mm_storer_ps(result.elements, _mm_add_ps(_left, _right));
	return result;
}

const yuki_vec4
vec4_sub
(const yuki_vec4 left, const yuki_vec4 right)
{
	__m128	_left = _mm_loadr_ps(left.elements),
		_right = _mm_loadr_ps(right.elements);

	yuki_vec4 result;
	_mm_storer_ps(result.elements, _mm_sub_ps(_left, _right));
	return result;
}

const yuki_vec4
vec4_mul_scalar
(const yuki_vec4 vector, const f32 scalar)
{
	__m128	_left = _mm_loadr_ps(vector.elements),
		_right = _mm_set1_ps(scalar);

	yuki_vec4 result;
	_mm_storer_ps(result.elements, _mm_mul_ps(_left, _right));
	return result;
}

const yuki_vec4
vec4_div_scalar
(const yuki_vec4 vector, const f32 scalar)
{
	__m128	_left = _mm_loadr_ps(vector.elements),
		_right = _mm_set1_ps(scalar);

	yuki_vec4 result;
	_mm_storer_ps(result.elements, _mm_div_ps(_left, _right));
	return result;
}

const yuki_vec4
vec4_mul_vec4
(const yuki_vec4 left, const yuki_vec4 right)
{
	__m128	_left = _mm_loadr_ps(left.elements),
		_right = _mm_loadr_ps(right.elements);

	yuki_vec4 result;
	_mm_storer_ps(result.elements, _mm_mul_ps(_left, _right));
	return result;
}

const yuki_vec4
vec4_div_vec4
(const yuki_vec4 left, const yuki_vec4 right)
{
	__m128	_left = _mm_loadr_ps(left.elements),
		_right = _mm_loadr_ps(right.elements),
		_zero = _mm_set1_ps(0);

	yuki_vec4 result;

	if (right.x == 0 || right.y == 0 || right.z == 0 || right.w == 0)
		_mm_store_ps(result.elements, _zero);
	else
		_mm_storer_ps(result.elements, _mm_mul_ps(_left, _right));

	return result;
}

const yuki_vec4
vec4_mul_mat4
(yuki_vec4 vector, const yuki_mat4 matrix)
{
	yuki_mat4 mat = mat4_get_transposed(matrix);

	__m128 _res =
		_mm_add_ps(
			_mm_add_ps(
				_mm_mul_ps(_mm_load_ps(mat.elements + 0), _mm_set1_ps(vector.x)),
				_mm_mul_ps(_mm_load_ps(mat.elements + 4), _mm_set1_ps(vector.y))
			),
			_mm_add_ps(
				_mm_mul_ps(_mm_load_ps(mat.elements + 8), _mm_set1_ps(vector.z)),
				_mm_mul_ps(_mm_load_ps(mat.elements + 12), _mm_set1_ps(vector.w))
			)
		);

	_mm_store_ps(vector.elements, _res);
	return vector;
}

const f32
vec4_get_magnitude
(const yuki_vec4 vector)
{
	return sqrtf((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z) + (vector.w * vector.w));
}

bool
vec4_normalize
(yuki_vec4 *vector)
{
	if (!vector)
		return false;

	__m128	_left = _mm_loadr_ps(vector->elements),
		_right = _mm_set1_ps(vec4_get_magnitude(*vector));

	_mm_storer_ps(vector->elements, _mm_div_ps(_left, _right));

	return true;
}

const yuki_vec4
vec4_get_normalized
(yuki_vec4 vector)
{
	vec4_normalize(&vector);
	return vector;
}


// vector conversion ///////////////////////////////////////////////////////

const yuki_vec2
vec2_from_vec3
(const yuki_vec3 vector)
{
	return (yuki_vec2) { vector.x, vector.y };
}

const yuki_vec2
vec2_from_vec4
(const yuki_vec4 vector)
{
	return (yuki_vec2) { vector.x, vector.y };
}

const yuki_vec3
vec3_from_vec2
(const yuki_vec2 vector)
{
	return (yuki_vec3) { vector.x, vector.y, 0.0f };
}

const yuki_vec3
vec3_from_vec4
(const yuki_vec4 vector)
{
	return (yuki_vec3) { vector.x, vector.y, vector.z };
}

const yuki_vec4
vec4_from_vec2
(const yuki_vec2 vector, const f32 w)
{
	return (yuki_vec4) { vector.x, vector.y, 0.0f, w };
}

const yuki_vec4
vec4_from_vec3
(const yuki_vec3 vector, const f32 w)
{
	return (yuki_vec4) { vector.x, vector.y, vector.z, w };
}





