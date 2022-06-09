#ifndef YUKI_HEADER_MEM
#define YUKI_HEADER_MEM

#include "yuki/broadcast.h"


typedef enum e_yuki_memory_tag {

	YUKI_MEMORY_TAG_UNKNOWN,
	YUKI_MEMORY_TAG_STATIC_ARRAY,
	YUKI_MEMORY_TAG_DYNAMIC_ARRAY,
	YUKI_MEMORY_TAG_ALLOCATORS,
	YUKI_MEMORY_TAG_GAME,
	YUKI_MEMORY_TAG_APPLICATION,
	YUKI_MEMORY_TAG_STRING,
	YUKI_MEMORY_TAG_TEXTURE,
	YUKI_MEMORY_TAG_MATERIAL_INSTANCE,
	YUKI_MEMORY_TAG_GEOMETRY,
	YUKI_MEMORY_TAG_RENDERER,

	YUKI_MEMORY_TAGS_NUM

}
yuki_memory_tag;


YUKI_API bool
memory_module_startup
(u64 *required_memory_size, void *state);

YUKI_API bool
memory_module_shutdown
(void *state);


YUKI_API void *
memory_module_allocate_block
(const u64 block_size, const yuki_memory_tag tag);

YUKI_API void
memory_module_deallocate_block
(void *memory_block, const u64 block_size, const yuki_memory_tag tag);


YUKI_API void *
memory_module_set_block_zero
(void *memory_block, const u64 block_size);

YUKI_API void *
memory_module_set_block_value
(void *memory_block, const u64 block_size, const s32 value);

YUKI_API void *
memory_module_copy_block
(void *dst_block, void *src_block, const u64 block_size);


YUKI_API const_str
memory_module_get_usage_info
(void);

YUKI_API const u32
memory_module_get_alloc_count
(void);



#endif
