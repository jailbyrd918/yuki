#ifndef YUKI_HEADER_MATH_DEFS
#define YUKI_HEADER_MATH_DEFS

#include "yuki/broadcast.h"


#define YUKI_MATH_MAX(left, right)			((left > right) ? left : right)
#define YUKI_MATH_MIN(left, right)			((left < right) ? left : right)
#define YUKI_MATH_CLAMP(value, min_value, max_value)	(YUKI_MATH_MIN(YUKI_MATH_MAX(value, min_value), max_value))



#endif
