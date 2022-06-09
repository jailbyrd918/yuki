#ifndef YUKI_HEADER_MATRIX
#define YUKI_HEADER_MATRIX

#include "yuki/math/math_defs.h"


const yuki_mat4
mat4_get_identity
(void);

const yuki_mat4
mat4_get_transposed
(const yuki_mat4 matrix);

const yuki_mat4
mat4_create_translation_from_xyz
(const f32 scale_x, const f32 scale_y, const f32 scale_z);

const yuki_mat4
mat4_create_translation_from_vector
(const yuki_vec3 vector);

const yuki_mat4
mat4_create_rotation_x
(const f32 angle_radians);

const yuki_mat4
mat4_create_rotation_y
(const f32 angle_radians);

const yuki_mat4
mat4_create_rotation_z
(const f32 angle_radians);

const yuki_mat4
mat4_create_rotation_from_xyz
(const f32 x_radians, const f32 y_radians, const f32 z_radians);

const yuki_mat4
mat4_create_rotation_from_matrices
(const yuki_mat4 x_rotation_matrix, const yuki_mat4 y_rotation_matrix, const yuki_mat4 z_rotation_matrix);

const yuki_mat4
mat4_create_scale_from_xyz
(const f32 scale_x, const f32 scale_y, const f32 scale_z);

const yuki_mat4
mat4_create_scale_from_vector
(const yuki_vec3 vector);

const yuki_mat4
mat4_create_perspective
(const f32 fov_radians, const f32 aspect_ratio, const f32 near_plane, const f32 far_plane);

const yuki_mat4
mat4_create_look_at
(const yuki_vec3 position, const yuki_vec3 target, const yuki_vec3 up);

const yuki_mat4
mat4_mul_mat4
(const yuki_mat4 left, const yuki_mat4 right);


#endif
