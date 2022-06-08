#ifndef YUKI_HEADER_MATRIX
#define YUKI_HEADER_MATRIX

#include "yuki/math/math_defs.h"


const mat4
mat4_get_identity
(void);

const mat4
mat4_get_transposed
(const mat4 matrix);

const mat4
mat4_create_translation_from_xyz
(const f32 scale_x, const f32 scale_y, const f32 scale_z);

const mat4
mat4_create_translation_from_vector
(const vec3 vector);

const mat4
mat4_create_rotation_x
(const f32 angle_radians);

const mat4
mat4_create_rotation_y
(const f32 angle_radians);

const mat4
mat4_create_rotation_z
(const f32 angle_radians);

const mat4
mat4_create_rotation_from_xyz
(const f32 x_radians, const f32 y_radians, const f32 z_radians);

const mat4
mat4_create_rotation_from_matrices
(const mat4 x_rotation_matrix, const mat4 y_rotation_matrix, const mat4 z_rotation_matrix);

const mat4
mat4_create_scale_from_xyz
(const f32 scale_x, const f32 scale_y, const f32 scale_z);

const mat4
mat4_create_scale_from_vector
(const vec3 vector);

const mat4
mat4_create_perspective
(const f32 fov_radians, const f32 aspect_ratio, const f32 near_plane, const f32 far_plane);

const mat4
mat4_create_look_at
(const vec3 position, const vec3 target, const vec3 up);

const mat4
mat4_mul_mat4
(const mat4 left, const mat4 right);


#endif
