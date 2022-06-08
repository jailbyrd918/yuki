#ifndef YUKI_HEADER_VECTOR
#define YUKI_HEADER_VECTOR

#include "yuki/math/math_defs.h"


// 2d vector ///////////////////////////////////////////////////////////////

const vec2
vec2_new
(const f32 x, const f32 y);

const vec2
vec2_copy
(const vec2 vector);

const vec2
vec2_zero
(void);

const vec2
vec2_one
(void);

const vec2
vec2_up
(void);

const vec2
vec2_down
(void);

const vec2
vec2_left
(void);

const vec2
vec2_right
(void);

const vec2
vec2_add
(const vec2 left, const vec2 right);

const vec2
vec2_sub
(const vec2 left, const vec2 right);

const vec2
vec2_mul_scalar
(const vec2 vector, const f32 scalar);

const vec2
vec2_div_scalar
(const vec2 vector, const f32 scalar);

const vec2
vec2_mul_vec2
(const vec2 left, const vec2 right);

const vec2
vec2_div_vec2
(const vec2 left, const vec2 right);

const f32
vec2_get_magnitude
(const vec2 vector);

bool
vec2_normalize
(vec2 *vector);

const vec2
vec2_get_normalized
(vec2 vector);

const f32
vec2_dot_product
(const vec2 left, const vec2 right);


// 3d vector ///////////////////////////////////////////////////////////////

const vec3
vec3_new
(const f32 x, const f32 y, const f32 z);

const vec3
vec3_copy
(const vec3 vector);

const vec3
vec3_zero
(void);

const vec3
vec3_one
(void);

const vec3
vec3_up
(void);

const vec3
vec3_down
(void);

const vec3
vec3_left
(void);

const vec3
vec3_right
(void);

const vec3
vec3_forward
(void);

const vec3
vec3_backward
(void);

const vec3
vec3_add
(const vec3 left, const vec3 right);

const vec3
vec3_sub
(const vec3 left, const vec3 right);

const vec3
vec3_mul_scalar
(const vec3 vector, const f32 scalar);

const vec3
vec3_div_scalar
(const vec3 vector, const f32 scalar);

const vec3
vec3_mul_vec3
(const vec3 left, const vec3 right);

const vec3
vec3_div_vec3
(const vec3 left, const vec3 right);

const f32
vec3_get_magnitude
(const vec3 vector);

bool
vec3_normalize
(vec3 *vector);

const vec3
vec3_get_normalized
(vec3 vector);

const f32
vec3_dot_product
(const vec3 left, const vec3 right);

const vec3
vec3_cross_product
(const vec3 left, const vec3 right);


// 4d vector ///////////////////////////////////////////////////////////////

const vec4
vec4_new
(const f32 x, const f32 y, const f32 z, const f32 w);

const vec4
vec4_copy
(const vec4 vector);

const vec4
vec4_zero
(void);

const vec4
vec4_one
(void);

const vec4
vec4_add
(const vec4 left, const vec4 right);

const vec4
vec4_sub
(const vec4 left, const vec4 right);

const vec4
vec4_mul_scalar
(const vec4 vector, const f32 scalar);

const vec4
vec4_div_scalar
(const vec4 vector, const f32 scalar);

const vec4
vec4_mul_vec4
(const vec4 left, const vec4 right);

const vec4
vec4_div_vec4
(const vec4 left, const vec4 right);

const vec4
vec4_mul_mat4
(vec4 vector, const mat4 matrix);

const f32
vec4_get_magnitude
(const vec4 vector);

bool
vec4_normalize
(vec4 *vector);

const vec4
vec4_get_normalized
(vec4 vector);


// vector conversion ///////////////////////////////////////////////////////

const vec2
vec2_from_vec3
(const vec3 vector);

const vec2
vec2_from_vec4
(const vec4 vector);

const vec3
vec3_from_vec2
(const vec2 vector);

const vec3
vec3_from_vec4
(const vec4 vector);

const vec4
vec4_from_vec2
(const vec2 vector, const f32 w);

const vec4
vec4_from_vec3
(const vec3 vector, const f32 w);



#endif
