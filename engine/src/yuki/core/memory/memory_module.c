#include "yuki/core/debug_log.h"

#include "yuki/core/memory/memory_module.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct s_yuki_memory_storage {

	u32	total_allocated,
		tagged_alloc_list[YUKI_MEMORY_TAGS_NUM];

}
yuki_memory_storage;

#	define	KILOBYTES_IN_BYTES	(1024)
#	define	MEGABYTES_IN_BYTES	(1024 * 1024)
#	define	GIGABYTES_IN_BYTES	(1024 * 1024 * 1024)

typedef struct s_yuki_memory_subsystem_state {

	yuki_memory_storage		storage;
	u32				alloc_count;

}
yuki_memory_subsystem_state;

static const_str mem_tag_str_list[YUKI_MEMORY_TAGS_NUM] = {
	"UNKNOWN    ",
	"STATIC_ARR ",
	"DYNAMIC_ARR",
	"ALLOCATORS ",
	"GAME       ",
	"APPLICATION",
	"STRING     ",
	"TEXTURE    ",
	"MAT_INST   ",
	"RENDERER   ",
};

static yuki_memory_subsystem_state *state_ref;


YUKI_API bool
memory_module_startup
(u32 *required_memory_size, void *state)
{
	*required_memory_size = sizeof(yuki_memory_subsystem_state);

	if (!state)
		return false;

	state_ref = YUKI_CAST(yuki_memory_subsystem_state *, state);
	state_ref->alloc_count = 0;
	memory_module_set_block_zero(&state_ref->storage, sizeof(yuki_memory_storage));

	return true;
}

YUKI_API bool
memory_module_shutdown
(void *state)
{
	if (!state)
		return false;

	state_ref = YUKI_CAST(yuki_memory_subsystem_state *, state);

	if (state_ref->storage.total_allocated > 0) {
		for (s32 i = 0; i < YUKI_MEMORY_TAGS_NUM; ++i)
			if (state_ref->storage.tagged_alloc_list[i] > 0)
				YUKI_LOG_WARNING("memory blocks of tag '%s' are not fully deallocated.", mem_tag_str_list[i]);
	}
	else
		YUKI_LOG_INFO("memory blocks of all tags are fully deallocated.");

	state_ref = NULL;
	return true;
}


YUKI_API void *
memory_module_allocate_block
(const u32 block_size, const yuki_memory_tag tag)
{
	if (tag == YUKI_MEMORY_TAG_UNKNOWN)
		YUKI_LOG_WARNING("allocated a memory block under tag 'YUKI_MEMORY_TAG_UNKNOWN' - reclassify allocation.");

	// update memory storage
	if (state_ref) {
		state_ref->storage.total_allocated += block_size;
		state_ref->storage.tagged_alloc_list[tag] += block_size;
		++state_ref->alloc_count;
	}

	// allocate and return new block
	void *newblock = malloc(block_size);
	return newblock;
}

YUKI_API void
memory_module_deallocate_block
(void *memory_block, const u32 block_size, const yuki_memory_tag tag)
{
	if (tag == YUKI_MEMORY_TAG_UNKNOWN)
		YUKI_LOG_WARNING("reallocated a memory block under tag 'YUKI_MEMORY_TAG_UNKNOWN' - reclassify allocation.");

	// update memory storage
	if (state_ref) {
		state_ref->storage.total_allocated -= block_size;
		state_ref->storage.tagged_alloc_list[tag] -= block_size;
	}

	free(memory_block);
	memory_block = NULL;
}


YUKI_API void *
memory_module_set_block_zero
(void *memory_block, const u32 block_size)
{
	return memset(memory_block, 0, block_size);
}

YUKI_API void *
memory_module_set_block_value
(void *memory_block, const u32 block_size, const s32 value)
{
	return memset(memory_block, value, block_size);
}

YUKI_API void *
memory_module_copy_block
(void *dst_block, void *src_block, const u32 block_size)
{
	return memcpy(dst_block, src_block, block_size);
}


YUKI_API const_str
memory_module_get_usage_info
(void)
{
	char	buf[YUKI_LOG_ENTRY_BUFFER_MAX] = "\n============= Memory Usage =============\n";
	u32	bufoffset = strlen(buf);

	if (state_ref) {
		for (size_t i = 0; i < YUKI_MEMORY_TAGS_NUM; ++i) {
			char	unit[] = "_iB";
			f32	usage = 0.f;

			if (state_ref->storage.tagged_alloc_list[i] >= GIGABYTES_IN_BYTES) {
				unit[0] = 'G';
				usage = YUKI_CAST(f32, state_ref->storage.tagged_alloc_list[i] / GIGABYTES_IN_BYTES);
			}
			else if (state_ref->storage.tagged_alloc_list[i] >= MEGABYTES_IN_BYTES) {
				unit[0] = 'M';
				usage = YUKI_CAST(f32, state_ref->storage.tagged_alloc_list[i] / MEGABYTES_IN_BYTES);
			}
			else if (state_ref->storage.tagged_alloc_list[i] >= KILOBYTES_IN_BYTES) {
				unit[0] = 'K';
				usage = YUKI_CAST(f32, state_ref->storage.tagged_alloc_list[i] / KILOBYTES_IN_BYTES);
			}
			else {
				unit[0] = 'B';
				unit[1] = '\0';
				usage = YUKI_CAST(f32, state_ref->storage.tagged_alloc_list[i]);
			}

			bufoffset += snprintf(buf + bufoffset, YUKI_LOG_ENTRY_BUFFER_MAX, "%s\t-\t%.2f %s\n", mem_tag_str_list[i], usage, unit);
		}
	}

	return strdup(buf);
}

YUKI_API const u32
memory_module_get_alloc_count
(void)
{
	return (state_ref ? state_ref->alloc_count : 0);
}






