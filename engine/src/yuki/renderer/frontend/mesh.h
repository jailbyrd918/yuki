#ifndef YUKI_HEADER_MESH
#define YUKI_HEADER_MESH

#include "yuki/math/math_defs.h"
#include "yuki/renderer/renderer_defs.h"
#include "yuki/resources/resource_defs.h"


typedef struct s_yuki_mesh_module_config {

	u32	max_meshes_count;

}
yuki_mesh_module_config;


bool
mesh_module_startup
(u64 *required_memory_size, void *state, const yuki_mesh_module_config config);

bool
mesh_module_shutdown
(void *state);


yuki_mesh *
mesh_module_acquire_mesh
(const_str name);

bool
mesh_module_release_mesh
(const_str name);


#endif
