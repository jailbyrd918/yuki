#ifndef YUKI_HEADER_DYNAMIC_ARRAY
#define YUKI_HEADER_DYNAMIC_ARRAY

#include "yuki/broadcast.h"


typedef struct s_yuki_dynamic_array_header {

	u32	elem_size,
		length,
		capacity;

}
yuki_dynamic_array_header;

#define	YUKI_DYNARR_HEADER_LENGTH	3
#define	YUKI_DYNARR_DEFAULT_CAPACITY	10


YUKI_API void *
_ykimpl_dynamic_array_allocate
(const u32 element_size, const u32 capacity);

YUKI_API bool
_ykimpl_dynamic_array_deallocate
(void *dynamic_array);

YUKI_API void *
_ykimpl_dynamic_array_resize
(void *dynamic_array);

inline YUKI_API yuki_dynamic_array_header *
_ykimpl_dynamic_array_get_header
(void *dynamic_array);


#define	dynamic_array_alloc(type)	\
	_ykimpl_dynamic_array_allocate(sizeof(type), YUKI_DYNARR_DEFAULT_CAPACITY)

#define	dynamic_array_alloc_reserve_capacity(type, capacity)	\
	_ykimpl_dynamic_array_allocate(sizeof(type), capacity)

#define	dynamic_array_free(dynamic_array)	\
	_ykimpl_dynamic_array_deallocate(dynamic_array)

#define	dynamic_array_push_back(dynamic_array, ...)	\
	{	\
		yuki_dynamic_array_header *header = _ykimpl_dynamic_array_get_header(dynamic_array);	\
		if (header->length >= header->capacity) {	\
			dynamic_array = _ykimpl_dynamic_array_resize(dynamic_array);	\
		}	\
		header = _ykimpl_dynamic_array_get_header(dynamic_array);	\
		*(dynamic_array + header->length++) = __VA_ARGS__;	\
	}

#define	dynamic_array_pop_back(dynamic_array)	\
	{	\
		yuki_dynamic_array_header *header = _ykimpl_dynamic_array_get_header(dynamic_array);	\
		if (header->length > 0) {	\
			header->length--;	\
		}	\
	}

#define	dynamic_array_push_front(dynamic_array, ...)	\
	{	\
		yuki_dynamic_array_header *header = _ykimpl_dynamic_array_get_header(dynamic_array);	\
		if (header->length >= header->capacity) {	\
			dynamic_array = _ykimpl_dynamic_array_resize(dynamic_array);	\
		}	\
		header = _ykimpl_dynamic_array_get_header(dynamic_array);	\
		memory_module_copy_block(dynamic_array + 1, dynamic_array, header->elem_size * header->length);	\
		*dynamic_array = __VA_ARGS__;	\
		header->length++;	\
	}

#define	dynamic_array_pop_front(dynamic_array)	\
	{	\
		yuki_dynamic_array_header *header = _ykimpl_dynamic_array_get_header(dynamic_array);	\
		if (header->length > 0) {	\
			memory_module_copy_block(dynamic_array, dynamic_array + 1, header->elem_size * --header->length);	\
		}	\
	}

#define	dynamic_array_insert(dynamic_array, index, ...)	\
	{	\
		yuki_dynamic_array_header *header = _ykimpl_dynamic_array_get_header(dynamic_array);	\
		if ((index) >= 0 && (index) <= header->length) {	\
			if (header->length >= header->capacity) {	\
				dynamic_array = _ykimpl_dynamic_array_resize(dynamic_array);	\
			}	\
			header = _ykimpl_dynamic_array_get_header(dynamic_array);	\
			memory_module_copy_block(dynamic_array + (index) + 1, dynamic_array + (index), header->elem_size * (header->length - (index)));	\
			*(dynamic_array + (index)) = __VA_ARGS__;	\
			header->length++;	\
		}	\
	}

#define	dynamic_array_erase(dynamic_array, index)	\
	{	\
		yuki_dynamic_array_header *header = _ykimpl_dynamic_array_get_header(dynamic_array);	\
		if ((header->length > 0) && ((index) >= 0 && (index) < header->length)) {	\
			memory_module_copy_block(dynamic_array + (index), dynamic_array + (index) + 1, header->elem_size * (header->length - (index) - 1));	\
			header->length--;	\
		}	\
	}

#define	dynamic_array_clear(dynamic_array)	\
	(_ykimpl_dynamic_array_get_header(dynamic_array)->length = 0)

#define	dynamic_array_get_header(dynamic_array)	\
	(_ykimpl_dynamic_array_get_header(dynamic_array))


#define	dynamic_array_get_element_size(dynamic_array)	\
	(_ykimpl_dynamic_array_get_header(dynamic_array)->elem_size)

#define	dynamic_array_get_length(dynamic_array)	\
	(_ykimpl_dynamic_array_get_header(dynamic_array)->length)

#define	dynamic_array_get_capacity(dynamic_array)	\
	(_ykimpl_dynamic_array_get_header(dynamic_array)->capacity)



#endif
