#ifndef YUKI_HEADER_LINEAR_ALLOCATOR
#define YUKI_HEADER_LINEAR_ALLOCATOR

#include "yuki/broadcast.h"


typedef struct s_yuki_linear_allocator {

	u64	memory_capacity,
		allocated_size;

	void	*memory_block;

	bool	owns_memory;

}
yuki_linear_allocator;


YUKI_API bool
linear_allocator_construct
(yuki_linear_allocator *allocator, const u64 memory_capacity, void *memory_block);

YUKI_API bool
linear_allocator_destruct
(yuki_linear_allocator *allocator);


YUKI_API void *
linear_allocator_allocate
(yuki_linear_allocator *allocator, const u64 memory_size);

YUKI_API bool
linear_allocator_clear
(yuki_linear_allocator *allocator);



#endif
