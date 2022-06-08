#ifndef YUKI_HEADER_HASH_TABLE
#define YUKI_HEADER_HASH_TABLE

#include "yuki/broadcast.h"


typedef struct s_yuki_hash_table {

	u64	element_size;
	u32	element_count;

	void	*memory_block;

}
yuki_hash_table;


YUKI_API bool
hash_table_construct
(yuki_hash_table *table_ref, const u64 element_size, const u32 element_count, void *memory_block);

YUKI_API bool
hash_table_destruct
(yuki_hash_table *table_ref);


YUKI_API bool
hash_table_get
(yuki_hash_table *table_ref, const_str name, void *out);

YUKI_API bool
hash_table_set
(yuki_hash_table *table_ref, const_str name, void *value);

YUKI_API bool
hash_table_fill
(yuki_hash_table *table_ref, void *value);





#endif
