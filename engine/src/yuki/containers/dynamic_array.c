#include "yuki/core/debug_log.h"
#include "yuki/core/memory/memory_module.h"
#include "yuki/platform/filesystem.h"

#include "yuki/containers/dynamic_array.h"

#include <stdlib.h>
#include <string.h>


YUKI_API void *
_ykimpl_dynamic_array_allocate
(const u32 element_size, const u32 capacity)
{
	u32	headersz = sizeof(yuki_dynamic_array_header),
		dynamic_arraysz = element_size * capacity;

	// allocate new dynamic array
	void *newdynamic_array = memory_module_allocate_block(headersz + dynamic_arraysz, YUKI_MEMORY_TAG_DYNAMIC_ARRAY);
	memory_module_set_block_zero(newdynamic_array, headersz + dynamic_arraysz);

	// init header properties
	yuki_dynamic_array_header *header = YUKI_CAST(yuki_dynamic_array_header *, newdynamic_array);
	header->elem_size = element_size;
	header->length = 0;
	header->capacity = capacity;

	return YUKI_CAST(void *, header + YUKI_DYNARR_HEADER_LENGTH);
}

YUKI_API bool
_ykimpl_dynamic_array_deallocate
(void *dynamic_array)
{
	if (!dynamic_array) {
		YUKI_LOG_ERROR("dynamic array should not be null!");
		return false;
	}

	yuki_dynamic_array_header *header = _ykimpl_dynamic_array_get_header(dynamic_array);
	memory_module_deallocate_block(header, sizeof(yuki_dynamic_array_header) + (header->elem_size * header->capacity), YUKI_MEMORY_TAG_DYNAMIC_ARRAY);

	return true;
}

YUKI_API void *
_ykimpl_dynamic_array_resize
(void *dynamic_array)
{
	if (!dynamic_array) {
		YUKI_LOG_ERROR("dynamic array should not be null!");
		return NULL;
	}

	yuki_dynamic_array_header *header = _ykimpl_dynamic_array_get_header(dynamic_array);
	const u32 newcapacity = (header->capacity * 2) + (header->capacity == 0);

	void *newblock = _ykimpl_dynamic_array_allocate(header->elem_size, newcapacity);
	memory_module_copy_block(newblock, dynamic_array, header->elem_size * header->capacity);

	yuki_dynamic_array_header *newheader = _ykimpl_dynamic_array_get_header(newblock);
	newheader->length = header->length;

	memory_module_deallocate_block(header, sizeof(yuki_dynamic_array_header) + (header->elem_size * header->capacity), YUKI_MEMORY_TAG_DYNAMIC_ARRAY);
	return newblock;
}

inline YUKI_API yuki_dynamic_array_header *
_ykimpl_dynamic_array_get_header
(void *dynamic_array)
{
	return (YUKI_CAST(yuki_dynamic_array_header *, dynamic_array) - YUKI_DYNARR_HEADER_LENGTH);
}
