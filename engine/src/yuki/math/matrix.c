#include "yuki/math/vector.h"

#include "yuki/math/matrix.h"

#include <math.h>
#include <immintrin.h>


const yuki_mat4
mat4_get_identity
(void)
{
	yuki_mat4 mat = { 0.0f };
	mat.elements[0] = 1.0f;
	mat.elements[5] = 1.0f;
	mat.elements[10] = 1.0f;
	mat.elements[15] = 1.0f;

	return mat;
}

const yuki_mat4
mat4_get_transposed
(const yuki_mat4 matrix)
{
	__m128	_row1 = _mm_load_ps(matrix.elements + 0),
		_row2 = _mm_load_ps(matrix.elements + 4),
		_row3 = _mm_load_ps(matrix.elements + 8),
		_row4 = _mm_load_ps(matrix.elements + 12);

	_MM_TRANSPOSE4_PS(_row1, _row2, _row3, _row4);

	yuki_mat4 mat;
	_mm_store_ps(mat.elements + 0, _row1);
	_mm_store_ps(mat.elements + 4, _row2);
	_mm_store_ps(mat.elements + 8, _row3);
	_mm_store_ps(mat.elements + 12, _row4);
	return mat;
}

const yuki_mat4
mat4_create_translation_from_xyz
(const f32 scale_x, const f32 scale_y, const f32 scale_z)
{
	yuki_mat4 mat = mat4_get_identity();
	mat.elements[3] = scale_x;
	mat.elements[7] = scale_y;
	mat.elements[11] = scale_z;
	return mat;
}

const yuki_mat4
mat4_create_translation_from_vector
(const yuki_vec3 vector)
{
	yuki_mat4 mat = mat4_get_identity();
	mat.elements[3] = vector.x;
	mat.elements[7] = vector.y;
	mat.elements[11] = vector.z;
	return mat;
}

const yuki_mat4
mat4_create_rotation_x
(const f32 angle_radians)
{
	f32 c = cosf(angle_radians);
	f32 s = sinf(angle_radians);

	yuki_mat4 mat = mat4_get_identity();
	mat.elements[5] = c;
	mat.elements[6] = -s;
	mat.elements[9] = s;
	mat.elements[10] = c;
	return mat;
}

const yuki_mat4
mat4_create_rotation_y
(const f32 angle_radians)
{
	f32 c = cosf(angle_radians);
	f32 s = sinf(angle_radians);

	yuki_mat4 mat = mat4_get_identity();
	mat.elements[0] = c;
	mat.elements[2] = s;
	mat.elements[8] = -s;
	mat.elements[10] = c;
	return mat;
}

const yuki_mat4
mat4_create_rotation_z
(const f32 angle_radians)
{
	f32 c = cosf(angle_radians);
	f32 s = sinf(angle_radians);

	yuki_mat4 mat = mat4_get_identity();
	mat.elements[0] = c;
	mat.elements[1] = -s;
	mat.elements[4] = s;
	mat.elements[5] = c;
	return mat;
}

const yuki_mat4
mat4_create_rotation_from_xyz
(const f32 x_radians, const f32 y_radians, const f32 z_radians)
{
	yuki_mat4	rotx = mat4_create_rotation_x(x_radians),
		roty = mat4_create_rotation_y(y_radians),
		rotz = mat4_create_rotation_z(z_radians),
		mat;

	mat = mat4_mul_mat4(rotx, roty);
	mat = mat4_mul_mat4(mat, rotz);
	return mat;
}

const yuki_mat4
mat4_create_rotation_from_matrices
(const yuki_mat4 x_rotation_matrix, const yuki_mat4 y_rotation_matrix, const yuki_mat4 z_rotation_matrix)
{
	yuki_mat4 mat;
	mat = mat4_mul_mat4(x_rotation_matrix, y_rotation_matrix);
	mat = mat4_mul_mat4(mat, z_rotation_matrix);
	return mat;
}

const yuki_mat4
mat4_create_scale_from_xyz
(const f32 scale_x, const f32 scale_y, const f32 scale_z)
{
	yuki_mat4 mat = mat4_get_identity();
	mat.elements[0] = scale_x;
	mat.elements[5] = scale_y;
	mat.elements[10] = scale_z;
	return mat;
}

const yuki_mat4
mat4_create_scale_from_vector
(const yuki_vec3 vector)
{
	yuki_mat4 mat = mat4_get_identity();
	mat.elements[0] = vector.x;
	mat.elements[5] = vector.y;
	mat.elements[10] = vector.z;
	return mat;
}

const yuki_mat4
mat4_create_perspective
(const f32 fov_radians, const f32 aspect_ratio, const f32 near_plane, const f32 far_plane)
{
	const f32 halftanfov = tanf(fov_radians * .5f);

	yuki_mat4 mat = { 0.0f };
	mat.elements[0] = aspect_ratio * (1.0f / halftanfov);
	mat.elements[5] = 1.0f / halftanfov;
	mat.elements[10] = far_plane / (far_plane - near_plane);
	mat.elements[11] = (-far_plane * near_plane) / (far_plane - near_plane);
	mat.elements[14] = 1.0f;
	return mat;
}

const yuki_mat4
mat4_create_look_at
(const yuki_vec3 position, const yuki_vec3 target, const yuki_vec3 up)
{
	yuki_vec3	vecz = vec3_get_normalized(vec3_sub(target, position)),
		vecx = vec3_get_normalized(vec3_cross_product(up, vecz)),
		vecy = vec3_cross_product(vecz, vecx);

	yuki_mat4 mat = mat4_get_identity();

	// first row
	mat.elements[0] = vecx.x;
	mat.elements[1] = vecx.y;
	mat.elements[2] = vecx.z;
	mat.elements[3] = -vec3_dot_product(vecx, position);

	// second row
	mat.elements[4] = vecy.x;
	mat.elements[5] = vecy.y;
	mat.elements[6] = vecy.z;
	mat.elements[7] = -vec3_dot_product(vecy, position);

	// third row
	mat.elements[8] = vecz.x;
	mat.elements[9] = vecz.y;
	mat.elements[10] = vecz.z;
	mat.elements[11] = -vec3_dot_product(vecz, position);

	return mat;
}

const yuki_mat4
mat4_mul_mat4
(const yuki_mat4 left, const yuki_mat4 right)
{
	yuki_mat4 mat;

	for (s32 i = 0; i < 4; ++i) {
		__m128 _res =
			_mm_add_ps(
				_mm_add_ps(
					_mm_mul_ps(_mm_load_ps(left.elements + 0), _mm_set1_ps(right.elements[i * 4 + 0])),
					_mm_mul_ps(_mm_load_ps(left.elements + 4), _mm_set1_ps(right.elements[i * 4 + 1]))
				),
				_mm_add_ps(
					_mm_mul_ps(_mm_load_ps(left.elements + 8), _mm_set1_ps(right.elements[i * 4 + 2])),
					_mm_mul_ps(_mm_load_ps(left.elements + 12), _mm_set1_ps(right.elements[i * 4 + 3]))
				)
			);

		_mm_store_ps(mat.elements + (i * 4), _res);
	}

	return mat;
}


