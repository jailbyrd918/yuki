#include "yuki/core/debug_log.h"
#include "yuki/core/memory/memory_module.h"

#include "yuki/core/memory/linear_allocator.h"

#include <stdlib.h>


YUKI_API bool
linear_allocator_construct
(yuki_linear_allocator *allocator, const u64 memory_capacity, void *memory_block)
{
	if (!allocator) {
		YUKI_LOG_ERROR("linear allocator should not be null!");
		return false;
	}

	allocator->allocated_size = 0;
	allocator->memory_capacity = memory_capacity;
	allocator->owns_memory = (memory_block == NULL);
	allocator->memory_block = (memory_block == NULL) ? memory_module_allocate_block(memory_capacity, YUKI_MEMORY_TAG_ALLOCATORS) : memory_block;

	return true;
}

YUKI_API bool
linear_allocator_destruct
(yuki_linear_allocator *allocator)
{
	if (!allocator) {
		YUKI_LOG_ERROR("linear allocator is null!");
		return false;
	}

	allocator->allocated_size = 0;
	allocator->memory_capacity = 0;

	if (allocator->owns_memory && allocator->memory_block)
		memory_module_deallocate_block(allocator->memory_block, allocator->memory_capacity, YUKI_MEMORY_TAG_ALLOCATORS);

	allocator->memory_block = NULL;
	allocator->owns_memory = false;

	return true;
}


YUKI_API void *
linear_allocator_allocate
(yuki_linear_allocator *allocator, const u64 memory_size)
{
	if (!allocator || !allocator->memory_block) {
		YUKI_LOG_ERROR("given linear allocator is not initialized!");
		return NULL;
	}

	if (allocator->allocated_size + memory_size > allocator->memory_capacity) {
		u64 remain_size = allocator->memory_capacity - allocator->allocated_size;
		YUKI_LOG_ERROR("attempted to allocate %u bytes, only %u bytes remaining!", memory_size, remain_size);
		return NULL;
	}

	void *newblock = YUKI_CAST(u8 *, allocator->memory_block) + allocator->allocated_size;
	allocator->allocated_size += memory_size;
	return newblock;
}

YUKI_API bool
linear_allocator_clear
(yuki_linear_allocator *allocator)
{
	if (!allocator || !allocator->memory_block) {
		YUKI_LOG_ERROR("given linear allocator is not initialized!");
		return false;
	}

	allocator->allocated_size = 0;
	memory_module_set_block_zero(allocator->memory_block, allocator->memory_capacity);

	return true;
}