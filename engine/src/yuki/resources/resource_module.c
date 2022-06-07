#include "yuki/core/debug_log.h"
#include "yuki/resources/loaders/image_loader.h"

#include "yuki/resources/resource_module.h"

#include <stdlib.h>


typedef struct s_yuki_resource_module_state {

	yuki_resource_module_config	config;
	yuki_resource_loader		*registered_loaders;

}
yuki_resource_module_state;

static yuki_resource_module_state *state_ref;

static const_str resource_type_str_list[YUKI_RESOURCE_TYPES_NUM] = {
	"IMAGE"
};


static bool
_ykstatic_resource_module_load
(const_str name, yuki_resource_loader *loader, yuki_resource *resource_ref)
{
	if (!name || !loader || !resource_ref) {
		if (resource_ref)
			resource_ref->loader_id = YUKI_INVALID_ID_U32;

		return false;
	}

	resource_ref->loader_id = loader->id;
	return loader->pfn_load(loader, name, resource_ref);
}


bool
resource_module_startup
(u64 *required_memory_size, void *state, const yuki_resource_module_config config)
{
	if (config.register_loaders_count == 0) {
		YUKI_LOG_CRITICAL("there is no loader to register (register_loaders_count == 0)");
		return false;
	}

	*required_memory_size = sizeof(yuki_resource_module_state) + (sizeof(yuki_resource_loader) * config.register_loaders_count);

	if (!state)
		return false;

	state_ref = YUKI_CAST(yuki_resource_module_state *, state);
	state_ref->config = config;

	void *loadersblock = state_ref + sizeof(yuki_resource_module_state);
	state_ref->registered_loaders = loadersblock;

	// iterate through all loaders and invalidate them
	for (u32 i = 0; i < config.register_loaders_count; ++i)
		state_ref->registered_loaders[i].id = YUKI_INVALID_ID_U32;

	// register loaders
	{
		resource_module_register_loader(resouce_image_loader_construct());
	}

	return true;
}

void
resource_module_shutdown
(void *state)
{
	if (state_ref)
		state_ref = NULL;
}

YUKI_API bool
resource_module_register_loader
(const yuki_resource_loader loader)
{
	if (!state_ref)
		return false;

	u32 numloaders = state_ref->config.register_loaders_count;

	// ensure that all already existing loaders should not be registered
	for (u32 i = 0; i < numloaders; ++i) {
		yuki_resource_loader *ld = &state_ref->registered_loaders[i];

		if (ld->id == YUKI_INVALID_ID_U32)
			continue;

		if (ld->resrc_type == loader.resrc_type) {
			YUKI_LOG_ERROR("loader of resouce type '%s' already exists and thus will not be registered!", resource_type_str_list[loader.resrc_type]);
			return false;
		}
	}

	for (u32 i = 0; i < numloaders; ++i) {
		if (state_ref->registered_loaders[i].id != YUKI_INVALID_ID_U32)
			continue;

		state_ref->registered_loaders[i] = loader;
		state_ref->registered_loaders[i].id = i;

		YUKI_LOG_VERBOSE("loader of type '%s' registered.", resource_type_str_list[loader.resrc_type]);
		return true;
	}

	return false;
}

YUKI_API bool
resource_module_load
(yuki_resource *resource_ref, const_str name, const yuki_resource_type resource_type)
{
	if (!state_ref) {
		resource_ref->loader_id = YUKI_INVALID_ID_U32;
		YUKI_LOG_ERROR("no loader for type '%s' was found", resource_type_str_list[resource_type]);
		return false;
	}

	for (u32 i = 0; i < state_ref->config.register_loaders_count; ++i) {
		yuki_resource_loader *ld = &state_ref->registered_loaders[i];

		if (ld->id != YUKI_INVALID_ID_U32 && ld->resrc_type == resource_type)
			return _ykstatic_resource_module_load(name, ld, resource_ref);
	}

	return false;
}

YUKI_API void
resource_module_unload
(yuki_resource *resource_ref)
{
	if (state_ref && resource_ref && resource_ref->loader_id != YUKI_INVALID_ID_U32) {
		yuki_resource_loader *ld = &state_ref->registered_loaders[resource_ref->loader_id];

		if (ld->id != YUKI_INVALID_ID_U32 && ld->pfn_unload)
			ld->pfn_unload(ld, resource_ref);
	}
}


YUKI_API const_str
resouce_module_get_base_filepath
(void)
{
	if (!state_ref) {
		YUKI_LOG_ERROR("attempted to get asset base filepath before module startup - returning empty string!");
		return "";
	}

	return state_ref->config.asset_base_path;
}

