#ifndef YUKI_HEADER_VECTOR
#define YUKI_HEADER_VECTOR

#include "yuki/math/math_defs.h"


// 2d vector ///////////////////////////////////////////////////////////////

const yuki_vec2
vec2_new
(const f32 x, const f32 y);

const yuki_vec2
vec2_copy
(const yuki_vec2 vector);

const yuki_vec2
vec2_zero
(void);

const yuki_vec2
vec2_one
(void);

const yuki_vec2
vec2_up
(void);

const yuki_vec2
vec2_down
(void);

const yuki_vec2
vec2_left
(void);

const yuki_vec2
vec2_right
(void);

const yuki_vec2
vec2_add
(const yuki_vec2 left, const yuki_vec2 right);

const yuki_vec2
vec2_sub
(const yuki_vec2 left, const yuki_vec2 right);

const yuki_vec2
vec2_mul_scalar
(const yuki_vec2 vector, const f32 scalar);

const yuki_vec2
vec2_div_scalar
(const yuki_vec2 vector, const f32 scalar);

const yuki_vec2
vec2_mul_vec2
(const yuki_vec2 left, const yuki_vec2 right);

const yuki_vec2
vec2_div_vec2
(const yuki_vec2 left, const yuki_vec2 right);

const f32
vec2_get_magnitude
(const yuki_vec2 vector);

bool
vec2_normalize
(yuki_vec2 *vector);

const yuki_vec2
vec2_get_normalized
(yuki_vec2 vector);

const f32
vec2_dot_product
(const yuki_vec2 left, const yuki_vec2 right);


// 3d vector ///////////////////////////////////////////////////////////////

const yuki_vec3
vec3_new
(const f32 x, const f32 y, const f32 z);

const yuki_vec3
vec3_copy
(const yuki_vec3 vector);

const yuki_vec3
vec3_zero
(void);

const yuki_vec3
vec3_one
(void);

const yuki_vec3
vec3_up
(void);

const yuki_vec3
vec3_down
(void);

const yuki_vec3
vec3_left
(void);

const yuki_vec3
vec3_right
(void);

const yuki_vec3
vec3_forward
(void);

const yuki_vec3
vec3_backward
(void);

const yuki_vec3
vec3_add
(const yuki_vec3 left, const yuki_vec3 right);

const yuki_vec3
vec3_sub
(const yuki_vec3 left, const yuki_vec3 right);

const yuki_vec3
vec3_mul_scalar
(const yuki_vec3 vector, const f32 scalar);

const yuki_vec3
vec3_div_scalar
(const yuki_vec3 vector, const f32 scalar);

const yuki_vec3
vec3_mul_vec3
(const yuki_vec3 left, const yuki_vec3 right);

const yuki_vec3
vec3_div_vec3
(const yuki_vec3 left, const yuki_vec3 right);

const f32
vec3_get_magnitude
(const yuki_vec3 vector);

bool
vec3_normalize
(yuki_vec3 *vector);

const yuki_vec3
vec3_get_normalized
(yuki_vec3 vector);

const f32
vec3_dot_product
(const yuki_vec3 left, const yuki_vec3 right);

const yuki_vec3
vec3_cross_product
(const yuki_vec3 left, const yuki_vec3 right);


// 4d vector ///////////////////////////////////////////////////////////////

const yuki_vec4
vec4_new
(const f32 x, const f32 y, const f32 z, const f32 w);

const yuki_vec4
vec4_copy
(const yuki_vec4 vector);

const yuki_vec4
vec4_zero
(void);

const yuki_vec4
vec4_one
(void);

const yuki_vec4
vec4_add
(const yuki_vec4 left, const yuki_vec4 right);

const yuki_vec4
vec4_sub
(const yuki_vec4 left, const yuki_vec4 right);

const yuki_vec4
vec4_mul_scalar
(const yuki_vec4 vector, const f32 scalar);

const yuki_vec4
vec4_div_scalar
(const yuki_vec4 vector, const f32 scalar);

const yuki_vec4
vec4_mul_vec4
(const yuki_vec4 left, const yuki_vec4 right);

const yuki_vec4
vec4_div_vec4
(const yuki_vec4 left, const yuki_vec4 right);

const yuki_vec4
vec4_mul_mat4
(yuki_vec4 vector, const yuki_mat4 matrix);

const f32
vec4_get_magnitude
(const yuki_vec4 vector);

bool
vec4_normalize
(yuki_vec4 *vector);

const yuki_vec4
vec4_get_normalized
(yuki_vec4 vector);


// vector conversion ///////////////////////////////////////////////////////

const yuki_vec2
vec2_from_vec3
(const yuki_vec3 vector);

const yuki_vec2
vec2_from_vec4
(const yuki_vec4 vector);

const yuki_vec3
vec3_from_vec2
(const yuki_vec2 vector);

const yuki_vec3
vec3_from_vec4
(const yuki_vec4 vector);

const yuki_vec4
vec4_from_vec2
(const yuki_vec2 vector, const f32 w);

const yuki_vec4
vec4_from_vec3
(const yuki_vec3 vector, const f32 w);



#endif
