#ifndef YUKI_HEADER_RESOURCE_MODULE
#define YUKI_HEADER_RESOURCE_MODULE

#include "yuki/resources/resource_defs.h"


bool
resource_module_startup
(u64 *required_memory_size, void *state, const yuki_resource_module_config config);

void
resource_module_shutdown
(void *state);


YUKI_API bool
resource_module_register_loader
(const yuki_resource_loader loader);

YUKI_API bool
resource_module_load
(yuki_resource *resource_ref, const_str name, const yuki_resource_type resource_type);

YUKI_API void
resource_module_unload
(yuki_resource *resource_ref);


YUKI_API const_str
resouce_module_get_base_filepath
(void);


#endif
