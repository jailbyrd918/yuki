#ifndef YUKI_HEADER_DEBUG_LOG
#define YUKI_HEADER_DEBUG_LOG

#include "yuki/broadcast.h"


typedef	enum e_yuki_log_level {

	YUKI_LOG_LEVEL_DEBUG,
	YUKI_LOG_LEVEL_VERBOSE,

	YUKI_LOG_LEVEL_INFO,
	YUKI_LOG_LEVEL_WARNING,
	YUKI_LOG_LEVEL_ERROR,
	YUKI_LOG_LEVEL_CRITICAL

}
yuki_log_level;

#define	YUKI_LOG_ENTRY_BUFFER_MAX	32767


bool
log_module_startup
(const_str log_output_filepath, u64 *required_memory_size, void *state);

bool
log_module_shutdown
(void *state);

YUKI_API void
log_output_print
(const yuki_log_level log_level, const_str filename, const s32 line, const_str message, ...);


#define	YUKI_LOG_CRITICAL(message, ...)	\
	log_output_print(YUKI_LOG_LEVEL_CRITICAL, filesystem_get_filename_from_filepath(__FILE__), __LINE__, message, __VA_ARGS__)

#define	YUKI_LOG_ERROR(message, ...)	\
	log_output_print(YUKI_LOG_LEVEL_ERROR, filesystem_get_filename_from_filepath(__FILE__), __LINE__, message, __VA_ARGS__)

#define	YUKI_LOG_WARNING(message, ...)	\
	log_output_print(YUKI_LOG_LEVEL_WARNING, filesystem_get_filename_from_filepath(__FILE__), __LINE__, message, __VA_ARGS__)

#define	YUKI_LOG_INFO(message, ...)	\
	log_output_print(YUKI_LOG_LEVEL_INFO, filesystem_get_filename_from_filepath(__FILE__), __LINE__, message, __VA_ARGS__)

#if	defined(YUKI_CONFIG_DEBUG)
#	define	YUKI_LOG_DEBUG(message, ...)	\
	log_output_print(YUKI_LOG_LEVEL_DEBUG, filesystem_get_filename_from_filepath(__FILE__), __LINE__, message, __VA_ARGS__)

#	define	YUKI_LOG_VERBOSE(message, ...)	\
	log_output_print(YUKI_LOG_LEVEL_VERBOSE, filesystem_get_filename_from_filepath(__FILE__), __LINE__, message, __VA_ARGS__)

#else
#	define	YUKI_LOG_DEBUG(message, ...)
#	define	YUKI_LOG_VERBOSE(message, ...)

#endif


#endif
