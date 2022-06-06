#include "yuki/platform/filesystem.h"
#include "yuki/core/memory/memory_module.h"

#include "yuki/core/debug_log.h"

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>


typedef struct s_yuki_log_module_state {

	yuki_file_handle	handle;

}
yuki_log_module_state;

static yuki_log_module_state *state_ref;



static bool
_ykstatic_append_to_output_log_file
(const_str message)
{
	if (!state_ref || !state_ref->handle.is_valid || !state_ref->handle.ref)
		return false;

	u32 len = strlen(message);
	if (!filesystem_write_data_to_file(&state_ref->handle, message, len))
		YUKI_LOG_ERROR("error writing to output log file!");

	return true;
}


bool
log_module_startup
(const_str log_output_filepath, u64 *required_memory_size, void *state)
{
	*required_memory_size = sizeof(yuki_log_module_state);

	if (!state)
		return false;

	state_ref = YUKI_CAST(yuki_log_module_state *, state);
	memory_module_set_block_zero(state_ref, sizeof(yuki_log_module_state));

	if (log_output_filepath && strlen(log_output_filepath) > 0) {
		if (!filesystem_open_file(log_output_filepath, YUKI_FILE_IO_MODE_WRITE, YUKI_FILE_MODE_TEXT, &state_ref->handle)) {
			YUKI_LOG_ERROR("error opening output log file!");
			return false;
		}
	}

	return (state_ref != NULL);
}

bool
log_module_shutdown
(void *state)
{
	if (!state)
		return false;

	state_ref = YUKI_CAST(yuki_log_module_state *, state);

	if (!filesystem_close_file(&state_ref->handle))
		YUKI_LOG_ERROR("error closing output log file!");

	state_ref->handle.ref = NULL;
	state_ref->handle.is_valid = false;

	state_ref = NULL;
	return true;
}

YUKI_API void
log_output_print
(const yuki_log_level log_level, const_str filename, const s32 line, const_str message, ...)
{
	const_str lvlmsg;
	switch (log_level) {
		case YUKI_LOG_LEVEL_CRITICAL:	lvlmsg = "CRITICAL";	break;
		case YUKI_LOG_LEVEL_ERROR:	lvlmsg = "ERROR   ";	break;
		case YUKI_LOG_LEVEL_WARNING:	lvlmsg = "WARNING ";	break;
		case YUKI_LOG_LEVEL_INFO:	lvlmsg = "INFO    ";	break;
		case YUKI_LOG_LEVEL_DEBUG:	lvlmsg = "DEBUG   ";	break;
		case YUKI_LOG_LEVEL_VERBOSE:	lvlmsg = "VERBOSE ";	break;
		default:						break;
	}

	char	fmtmsg[YUKI_LOG_ENTRY_BUFFER_MAX] = { '\0' },
		timemsg[YUKI_LOG_ENTRY_BUFFER_MAX] = { '\0' },
		outmsg[YUKI_LOG_ENTRY_BUFFER_MAX] = { '\0' };

	// construct formatted message
	{
		YUKI_VA_LIST ls;
		va_start(ls, message);
		vsnprintf(fmtmsg, YUKI_LOG_ENTRY_BUFFER_MAX, message, ls);
		va_end(ls);
	}

	// format time message
	{
		time_t now = time(NULL);
		struct tm *local = localtime(&now);
		strftime(timemsg, YUKI_LOG_ENTRY_BUFFER_MAX, "%F %T", local);
	}

	// format final output message
	sprintf(outmsg, "[%s] (%s : %d) %s>> %s\n", timemsg, filename, line, lvlmsg, fmtmsg);

	// print final output message to console with ANSI-escape coloring
	{
		const_str fmt;
		switch (log_level) {
			case YUKI_LOG_LEVEL_CRITICAL:
				fmt = "\x1b[38;2;255;255;255m\x1b[48;2;255;0;0m%s\x1b[0m";	// red background - white foreground
				break;

			case YUKI_LOG_LEVEL_ERROR:
				fmt = "\x1b[38;2;255;0;0m%s\x1b[0m";				// red foreground
				break;
			case YUKI_LOG_LEVEL_WARNING:
				fmt = "\x1b[38;2;229;229;16m%s\x1b[0m";				// bright yellow foreground
				break;

			case YUKI_LOG_LEVEL_DEBUG:
			case YUKI_LOG_LEVEL_VERBOSE:
				fmt = "\x1b[38;2;44;181;233m%s\x1b[0m";				// cyan foreground
				break;

			default:
				fmt = "\x1b[38;2;0;255;0m%s\x1b[0m";				// green foreground
				break;
		}

		fprintf(stdout, fmt, outmsg);
	}

	// print final output message to file
	if (state_ref->handle.is_valid)
		_ykstatic_append_to_output_log_file(outmsg);

}
