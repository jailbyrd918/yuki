#include "yuki/core/debug_log.h"
#include "yuki/core/memory/memory_module.h"
#include "yuki/core/str.h"

#include "yuki/containers/hash_table.h"

#include <stdlib.h>


YUKI_API bool
hash_table_construct
(yuki_hash_table *table_ref, const u64 element_size, const u32 element_count, void *memory_block)
{
	if (!table_ref || !memory_block) {
		YUKI_LOG_ERROR("reference to hash table or refernce to memory block is null!");
		return false;
	}
	else if (element_count == 0 || element_size == 0) {
		YUKI_LOG_ERROR("element size and element count must be positive non-zero values!");
		return false;
	}

	table_ref->element_count = element_count;
	table_ref->element_size = element_size;
	table_ref->memory_block = memory_block;
	memory_module_set_block_zero(table_ref->memory_block, element_size * element_count);

	return true;
}

YUKI_API bool
hash_table_destruct
(yuki_hash_table *table_ref)
{
	if (!table_ref) {
		YUKI_LOG_ERROR("reference to hash table is null!");
		return false;
	}

	memory_module_set_block_zero(table_ref->memory_block, table_ref->element_size * table_ref->element_count);
	return true;
}


YUKI_API bool
hash_table_get
(yuki_hash_table *table_ref, const_str name, void *out)
{
	if (!table_ref || !name || !out) {
		YUKI_LOG_ERROR("reference to %s is null!", !table_ref ? "table" : (!name ? "name" : "out value"));
		return false;
	}

	u64 hashval = str_hash_string(name);
	hashval %= table_ref->element_count;
	memory_module_copy_block(out, YUKI_CAST(intptr_t *, table_ref->memory_block) + (table_ref->element_size * hashval), table_ref->element_size);
	return true;
}

YUKI_API bool
hash_table_set
(yuki_hash_table *table_ref, const_str name, void *value)
{
	if (!table_ref || !name || !value) {
		YUKI_LOG_ERROR("reference to %s is null!", !table_ref ? "table" : (!name ? "name" : "value"));
		return false;
	}

	u64 hashval = str_hash_string(name);
	hashval %= table_ref->element_count;
	memory_module_copy_block(YUKI_CAST(intptr_t *, table_ref->memory_block) + (table_ref->element_size * hashval), value, table_ref->element_size);
	return true;
}

YUKI_API bool
hash_table_fill
(yuki_hash_table *table_ref, void *value)
{
	if (!table_ref || !value) {
		YUKI_LOG_ERROR("reference to %s is null!", !table_ref ? "table" : "value");
		return false;
	}

	for (u32 i = 0; i < table_ref->element_count; ++i)
		memory_module_copy_block(YUKI_CAST(intptr_t *, table_ref->memory_block) + (table_ref->element_size * i), value, table_ref->element_size);

	return true;
}


