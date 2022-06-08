#ifndef YUKI_HEADER_MATH_DEFS
#define YUKI_HEADER_MATH_DEFS

#include "yuki/broadcast.h"


#define YUKI_MATH_MAX(left, right)			((left > right) ? left : right)
#define YUKI_MATH_MIN(left, right)			((left < right) ? left : right)
#define YUKI_MATH_CLAMP(value, min_value, max_value)	(YUKI_MATH_MIN(YUKI_MATH_MAX(value, min_value), max_value))

#if	defined(M_PI)
#	define	YUKI_MATH_PI		(M_PI)
#else
#	define	YUKI_MATH_PI		(3.14159265358979323846f)
#endif

#define	YUKI_MATH_DEG2RAD(degree)	(degree * YUKI_MATH_PI / 180.0f)
#define	YUKI_MATH_RAD2DEG(radian)	(radian * 180.0f / YUKI_MATH_PI)


typedef	union u_vec2 {

	struct {

		// first component 
		union {
			f32	x, r, s, u;
		};

		// second component 
		union {
			f32	y, g, t, v;
		};

	};

	f32	elements[2];

}
vec2;

typedef	union u_vec3 {

	struct {

		// first component 
		union {
			f32	x, r, s, u;
		};

		// second component 
		union {
			f32	y, g, t, v;
		};

		// third component 
		union {
			f32	z, b, p, w;
		};

	};

	f32	elements[3];

}
vec3;

typedef	union u_vec4 {

	struct {

		// first component 
		union {
			f32	x, r, s;
		};

		// second component 
		union {
			f32	y, g, t;
		};

		// third component 
		union {
			f32	z, b, p;
		};

		// fourth component 
		union {
			f32	w, a, q;
		};

	};

	f32	elements[4];

}
vec4;

typedef	union u_mat4 {

	f32	elements[16];

}
mat4;



#endif
