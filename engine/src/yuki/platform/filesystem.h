#ifndef YUKI_HEADER_FILESYSTEM
#define YUKI_HEADER_FILESYSTEM

#include "yuki/broadcast.h"


typedef enum e_yuki_file_io_mode {

	YUKI_FILE_IO_MODE_READ,
	YUKI_FILE_IO_MODE_WRITE,
	YUKI_FILE_IO_MODE_READ_WRITE

}
yuki_file_io_mode;

typedef enum e_yuki_file_mode {
	
	YUKI_FILE_MODE_TEXT,
	YUKI_FILE_MODE_BINARY

}
yuki_file_mode;

typedef struct s_yuki_file_handle {

	void	*ref;
	bool	is_valid;

}
yuki_file_handle;


bool
filesystem_open_file
(const_str filepath, const yuki_file_io_mode io_mode, const yuki_file_mode mode, yuki_file_handle *handle);

bool
filesystem_close_file
(yuki_file_handle *handle);


const bool
filesystem_check_file_exists
(const_str filepath);

const u32
filesystem_get_file_size
(yuki_file_handle *handle);

bool
filesystem_read_line_from_file
(yuki_file_handle *handle, const u32 buffer_max_length, str *line_buffer, u32 *line_buffer_length);

bool
filesystem_write_line_to_file
(yuki_file_handle *handle, const_str text);

const_str
filesystem_read_all_text_from_file
(yuki_file_handle *handle, u32 *read_bytes);

bool
filesystem_write_data_to_file
(yuki_file_handle *handle, const void *data, const u32 data_size);

const_str
filesystem_get_filename_from_filepath
(const_str filepath);



#endif
