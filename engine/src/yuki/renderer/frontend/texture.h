#ifndef YUKI_HEADER_TEXTURE
#define YUKI_HEADER_TEXTURE

#include "yuki/renderer/renderer_defs.h"
#include "yuki/resources/resource_defs.h"


typedef struct s_yuki_texture_module_config {

	u32	max_textures_count;

}
yuki_texture_module_config;


bool
texture_module_startup
(u64 *required_memory_size, void *state, const yuki_texture_module_config config);

bool
texture_module_shutdown
(void *state);


yuki_texture *
texture_module_acquire_texture
(const_str name);

bool
texture_module_release_texture
(const_str name);



#endif
