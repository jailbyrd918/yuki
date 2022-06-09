#include "yuki/core/debug_log.h"
#include "yuki/core/memory/memory_module.h"
#include "yuki/core/str.h"
#include "yuki/containers/hash_table.h"
#include "yuki/renderer/frontend/render_module.h"
#include "yuki/resources/resource_module.h"

#include "yuki/renderer/frontend/mesh.h"

#include <stdlib.h>


typedef struct s_yuki_mesh_reference {

	u32	handle,
		ref_count;

	bool	auto_release;

}
yuki_mesh_reference;

typedef struct s_yuki_mesh_module_state {

	yuki_mesh_module_config	config;

	yuki_mesh		*registered_meshes;
	yuki_hash_table		registered_mesh_table;

}
yuki_mesh_module_state;

static yuki_mesh_module_state *state_ref;


static bool
_ykstatic_mesh_load
(yuki_mesh *mesh, const_str mesh_name)
{
	yuki_resource meshresrc;
	if (!resource_module_load(&meshresrc, mesh_name, YUKI_RESOURCE_TYPE_MESH)) {
		YUKI_LOG_ERROR("failed to load mesh resource for texture '%s'!", mesh_name);
		return false;
	}

	str_copy_string_ranged(mesh->name, mesh_name, str_get_string_length(mesh_name));

	yuki_mesh_resource_data *resrcdata = YUKI_CAST(yuki_mesh_resource_data *, meshresrc.data);
	u64 datasize =
		(sizeof(yuki_vec3) * resrcdata->vertices_count) +
		(sizeof(yuki_vec2) * resrcdata->uv_coordinates_count) + 
		(sizeof(yuki_mesh_face_data) * resrcdata->faces_count);

	mesh->data = memory_module_allocate_block(datasize, YUKI_MEMORY_TAG_GEOMETRY);
	memory_module_copy_block(mesh->data, resrcdata, datasize);

	resource_module_unload(&meshresrc);
	return true;
}

static bool
_ykstatic_mesh_destroy
(yuki_mesh *mesh)
{
	if (!mesh) {
		YUKI_LOG_ERROR("reference to mesh is null!");
		return false;
	}

	yuki_mesh_resource_data *resrcdata = YUKI_CAST(yuki_mesh_resource_data *, mesh->data);
	u64 datasize =
		(sizeof(yuki_vec3) * resrcdata->vertices_count) +
		(sizeof(yuki_vec2) * resrcdata->uv_coordinates_count) +
		(sizeof(yuki_mesh_face_data) * resrcdata->faces_count);

	memory_module_deallocate_block(mesh->data, datasize, YUKI_MEMORY_TAG_GEOMETRY);
	memory_module_set_block_zero(mesh, sizeof(yuki_mesh));
	mesh->id = YUKI_INVALID_ID_U32;
	return true;
}

static bool
_ykstatic_process_mesh_ref
(u32 *mesh_id_ref, const_str mesh_name, const s32 reference_diff, const bool auto_release)
{
	*mesh_id_ref = YUKI_INVALID_ID_U32;

	if (!state_ref)
		return false;

	yuki_mesh_reference meshref;
	if (!hash_table_get(&state_ref->registered_mesh_table, mesh_name, &meshref)) {
		YUKI_LOG_ERROR("failed to acquire id for mesh name '%s'!", mesh_name);
		return false;
	}

	if (meshref.ref_count == 0 && reference_diff > 0) {
		if (reference_diff > 0)
			meshref.auto_release = auto_release;
		else {
			if (meshref.auto_release) {
				YUKI_LOG_WARNING("attempted to release non-existent mesh '%s'.", mesh_name);
				return false;
			}
			else {
				YUKI_LOG_WARNING("attempted to release a name '%s' that has < auto_release = false >, ref_count already = 0.", mesh_name);
				return true;
			}
		}
	}

	meshref.ref_count += reference_diff;

	char tempname[YUKI_TEXTURE_NAME_MAX_LENGTH] = { '\0' };
	str_copy_string_ranged(tempname, mesh_name, str_get_string_length(mesh_name));

	if (reference_diff < 0) {
		if (meshref.ref_count == 0 && meshref.auto_release) {
			yuki_mesh *mesh = &state_ref->registered_meshes[meshref.handle];
			_ykstatic_mesh_destroy(mesh);

			meshref.handle = YUKI_INVALID_ID_U32;
			meshref.auto_release = false;
		}

		YUKI_LOG_VERBOSE("released mesh '%s'.", tempname);
	}
	else {
		// if the handle is new
		if (meshref.handle == YUKI_INVALID_ID_U32) {
			// find the available slot
			for (u32 i = 0; i < state_ref->config.max_meshes_count; ++i) {
				if (state_ref->registered_meshes[i].id == YUKI_INVALID_ID_U32) {
					*mesh_id_ref = meshref.handle = i;
					break;
				}
			}

			if (*mesh_id_ref == YUKI_INVALID_ID_U32) {
				YUKI_LOG_CRITICAL("mesh module cannot hold anymore meshes - configuration adjustment is required!");
				return false;
			}

			// create new mesh ...
			{
				yuki_mesh *mesh = &state_ref->registered_meshes[meshref.handle];
				if (!_ykstatic_mesh_load(mesh, mesh_name)) {
					*mesh_id_ref = YUKI_INVALID_ID_U32;
					YUKI_LOG_ERROR("failed to load mesh '%s'!", mesh_name);
					return false;
				}

				mesh->id = meshref.handle;
				YUKI_LOG_VERBOSE("created mesh '%s', now ref_count is %u", mesh_name, meshref.ref_count);
			}
		}
		else {
			*mesh_id_ref = meshref.handle;
			YUKI_LOG_VERBOSE("mesh '%s' already exists, ref_count increased to %u.", mesh_name, meshref.ref_count);
		}
	}

	hash_table_set(&state_ref->registered_mesh_table, mesh_name, &meshref);
	return true;
}


bool
mesh_module_startup
(u64 *required_memory_size, void *state, const yuki_mesh_module_config config)
{
	if (config.max_meshes_count == 0) {
		YUKI_LOG_ERROR("maximum mesh count must be a non-zero positive value!");
		return false;
	}

	u64	modulesize = sizeof(yuki_mesh_module_state),
		arrsize = sizeof(yuki_mesh) * config.max_meshes_count,
		htablesize = sizeof(yuki_mesh_reference) * config.max_meshes_count;

	
	*required_memory_size = modulesize + arrsize + htablesize;

	if (!state)
		return false;

	state_ref = YUKI_CAST(yuki_mesh_module_state *, state);
	state_ref->config = config;

	void *arrblock = state_ref + modulesize;
	state_ref->registered_meshes = arrblock;

	void *htblock = state_ref + modulesize + arrsize;
	hash_table_construct(&state_ref->registered_mesh_table, sizeof(yuki_mesh_reference), config.max_meshes_count, htblock);

	// fill hash table with invalid references for default
	yuki_mesh_reference invalidref;
	invalidref.handle = YUKI_INVALID_ID_U32;
	invalidref.ref_count = 0;
	hash_table_fill(&state_ref->registered_mesh_table, &invalidref);

	// invalidate all meshes in array
	for (u32 i = 0; i < state_ref->config.max_meshes_count; ++i)
		state_ref->registered_meshes[i].id = YUKI_INVALID_ID_U32;

	return true;
}

bool
mesh_module_shutdown
(void *state)
{
	if (!state)
		return false;

	hash_table_destruct(&state_ref->registered_mesh_table);
	state_ref = NULL;

	return true;
}


yuki_mesh *
mesh_module_acquire_mesh
(const_str name)
{
	if (!name) {
		YUKI_LOG_ERROR("given name is null - returning nullptr mesh!");
		return NULL;
	}

	u32 id = YUKI_INVALID_ID_U32;
	if (!_ykstatic_process_mesh_ref(&id, name, 1, true)) {
		YUKI_LOG_ERROR("failed to acquire mesh '%s'!", name);
		return NULL;
	}

	return &state_ref->registered_meshes[id];
}

bool
mesh_module_release_mesh
(const_str name)
{
	if (!name) {
		YUKI_LOG_ERROR("given name is null - returning nullptr mesh!");
		return false;
	}

	u32 id = YUKI_INVALID_ID_U32;
	if (!_ykstatic_process_mesh_ref(&id, name, -1, false)) {
		YUKI_LOG_ERROR("failed to release mesh '%s'!", name);
		return false;
	}

	return true;
}


