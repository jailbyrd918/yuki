#include "yuki/core/debug_log.h"

#include "yuki/platform/filesystem.h"

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>


bool
filesystem_open_file
(const_str filepath, const yuki_file_io_mode io_mode, const yuki_file_mode mode, yuki_file_handle *handle)
{
	handle->is_valid = false;
	handle->ref = NULL;

	const_str modestr;
	if ((io_mode & YUKI_FILE_IO_MODE_READ_WRITE) > 0)
		modestr = (mode & YUKI_FILE_MODE_BINARY) > 0 ? "w+b" : "w+";
	else if ((io_mode & YUKI_FILE_IO_MODE_READ) > 0)
		modestr = (mode & YUKI_FILE_MODE_BINARY) > 0 ? "rb" : "r";
	else if ((io_mode & YUKI_FILE_IO_MODE_WRITE) > 0)
		modestr = (mode & YUKI_FILE_MODE_BINARY) > 0 ? "wb" : "w";
	else {
		YUKI_LOG_ERROR("invalid modes provided to open a file from path '%s'", filepath);
		return false;
	}

	FILE *infile = fopen(filepath, modestr);
	if (!infile) {
		YUKI_LOG_ERROR("failed to open file from path '%s'", filepath);
		return false;
	}

	handle->ref = infile;
	handle->is_valid = true;

	return true;
}

bool
filesystem_close_file
(yuki_file_handle *handle)
{
	if (!handle->ref) {
		YUKI_LOG_ERROR("reference to file handle is null!");
		return false;
	}

	fclose(YUKI_CAST(FILE *, handle->ref));
	handle->ref = NULL;
	handle->is_valid = false;

	return true;
}


const bool
filesystem_check_file_exists
(const_str filepath)
{
#if	defined(_MSC_VER)			// MSVC compiler
	struct _stat buffer;
	return _stat(filepath, &buffer) == 0;

#else						// non-MSVC compiler
	struct stat buffer;
	return stat(filepath, &buffer) == 0;

#endif
}

const u32
filesystem_get_file_size
(yuki_file_handle *handle)
{
	if (!handle->ref) {
		YUKI_LOG_ERROR("reference to file handle is null!");
		return false;
	}

	u32	filesize;
	FILE	*file = YUKI_CAST(FILE *, handle->ref);

	fseek(file, 0, SEEK_END);
	filesize = ftell(file);
	fseek(file, 0, SEEK_SET);

	return filesize;
}

bool
filesystem_read_line_from_file
(yuki_file_handle *handle, const u32 buffer_max_length, str *line_buffer, u32 *line_buffer_length)
{
	if (!handle->ref) {
		YUKI_LOG_ERROR("reference to file handle is null!");
		return false;
	}

	if ((line_buffer && line_buffer_length) && (buffer_max_length > 0)) {
		if (fgets(*line_buffer, buffer_max_length, YUKI_CAST(FILE *, handle->ref)) != NULL) {
			*line_buffer_length = strlen(*line_buffer);
			return true;
		}
	}

	return false;
}

bool
filesystem_write_line_to_file
(yuki_file_handle *handle, const_str text)
{
	if (!handle->ref) {
		YUKI_LOG_ERROR("reference to file handle is null!");
		return false;
	}

	FILE *file = YUKI_CAST(FILE *, handle->ref);

	s32 res = fprintf(file, "%s", text);
	if (res != EOF)
		res = fputc('\n', file);

	fflush(file);
	return (res != EOF);
}

const_str
filesystem_read_all_text_from_file
(yuki_file_handle *handle, u32 *read_bytes)
{
	if (!handle->ref) {
		YUKI_LOG_ERROR("reference to file handle is null!");
		return NULL;
	}

	if (read_bytes) {
		str outtext = "";
		*read_bytes = fread(outtext, sizeof(char), filesystem_get_file_size(handle), YUKI_CAST(FILE *, handle->ref));
		return outtext;
	}

	return NULL;
}

bool
filesystem_write_data_to_file
(yuki_file_handle *handle, const void *data, const u32 data_size)
{
	if (!handle->ref) {
		YUKI_LOG_ERROR("reference to file handle is null!");
		return false;
	}

	FILE *file = YUKI_CAST(FILE *, handle->ref);
	fwrite(data, sizeof(u8), data_size, file);
	fflush(file);
	return true;
}

YUKI_API const_str
filesystem_get_filename_from_filepath
(const_str filepath)
{
	return (strrchr(filepath, '\\') ? strrchr(filepath, '\\') + 1 : strrchr(filepath, '/') ? strrchr(filepath, '/') + 1 : filepath);
}

