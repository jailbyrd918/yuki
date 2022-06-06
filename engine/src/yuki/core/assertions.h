#ifndef YUKI_HEADER_ASSERTIONS
#define YUKI_HEADER_ASSERTIONS

#include "yuki/core/debug_log.h"


#if	defined(YUKI_ENABLE_ASSERTIONS)

#	define	YUKI_ASSERT(expr)	\
		{	\
			if (!(expr)) {	\
				YUKI_LOG_CRITICAL("invalid expression '%s'", #expr);	\
				YUKI_DEBUG_BREAK();	\
			}	\
		}

#	define	YUKI_ASSERT_MSG(expr, msg)	\
		{	\
			if (!(expr)) {	\
				YUKI_LOG_CRITICAL("invalid expression '%s', message: %s", #expr, msg);	\
				YUKI_DEBUG_BREAK();	\
			}	\
		}

#else

#	define	YUKI_ASSERT(expr)
#	define	YUKI_ASSERT_MSG(expr, msg)

#endif



#endif
