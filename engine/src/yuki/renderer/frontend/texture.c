#include "yuki/core/debug_log.h"
#include "yuki/core/memory/memory_module.h"
#include "yuki/core/str.h"
#include "yuki/containers/hash_table.h"
#include "yuki/resources/resource_module.h"

#include "yuki/renderer/frontend/texture.h"

#include <stdlib.h>


typedef struct s_yuki_texture_module_state {

	yuki_texture_module_config	config;
	yuki_texture			default_texture;

	yuki_texture			*registered_textures;
	yuki_hash_table			registered_texture_table;

}
yuki_texture_module_state;

typedef struct s_yuki_texture_reference {

	u32	handle,
		ref_count;

	bool	auto_release;

}
yuki_texture_reference;

static yuki_texture_module_state *state_ref;


static bool
_ykstatic_texture_load
(yuki_texture *texture, const_str texture_name)
{
	yuki_resource imgresrc;
	if (!resource_module_load(&imgresrc, texture_name, YUKI_RESOURCE_TYPE_IMAGE)) {
		YUKI_LOG_ERROR("failed to load image resource for texture '%s'!", texture_name);
		return false;
	}

	yuki_image_resource_data *resrcdata = YUKI_CAST(yuki_image_resource_data *, imgresrc.data);

	u64 datasize = sizeof(color32) * resrcdata->width * resrcdata->height;

	str_copy_string_ranged(texture->name, texture_name, str_get_string_length(texture_name));
	texture->width = resrcdata->width;
	texture->height = resrcdata->height;
	texture->data = memory_module_allocate_block(datasize, YUKI_MEMORY_TAG_TEXTURE);
	memory_module_copy_block(texture->data, resrcdata->color_buffer, datasize);

	resource_module_unload(&imgresrc);
	return true;
}

static bool
_ykstatic_texture_destroy
(yuki_texture *texture)
{
	if (!texture) {
		YUKI_LOG_ERROR("reference to texture is null!");
		return false;
	}

	memory_module_deallocate_block(texture->data, sizeof(color32) * (texture->width * texture->height), YUKI_MEMORY_TAG_TEXTURE);
	memory_module_set_block_zero(texture, sizeof(yuki_texture));
	texture->id = YUKI_INVALID_ID_U32;
	return true;
}

static bool
_ykstatic_process_texture_ref
(u32 *texture_id_ref, const_str texture_name, const s32 reference_diff, const bool auto_release)
{
	*texture_id_ref = YUKI_INVALID_ID_U32;

	if (!state_ref)
		return false;

	yuki_texture_reference texref;
	if (!hash_table_get(&state_ref->registered_texture_table, texture_name, &texref)) {
		YUKI_LOG_ERROR("failed to acquire id for texture name '%s'!", texture_name);
		return false;
	}

	if (texref.ref_count == 0 && reference_diff > 0) {
		if (reference_diff > 0)
			texref.auto_release = auto_release;
		else {
			if (texref.auto_release) {
				YUKI_LOG_WARNING("attempted to release non-existent texture '%s'.", texture_name);
				return false;
			}
			else {
				YUKI_LOG_WARNING("attempted to release a texture '%s' that has < auto_release = false >, ref_count already = 0.", texture_name);
				return true;
			}
		}
	}

	texref.ref_count += reference_diff;

	char tempname[YUKI_TEXTURE_NAME_MAX_LENGTH] = { '\0' };
	str_copy_string_ranged(tempname, texture_name, str_get_string_length(texture_name));

	if (reference_diff < 0) {
		if (texref.ref_count == 0 && texref.auto_release) {
			yuki_texture *tex = &state_ref->registered_textures[texref.handle];
			_ykstatic_texture_destroy(tex);

			texref.handle = YUKI_INVALID_ID_U32;
			texref.auto_release = false;
		}

		YUKI_LOG_VERBOSE("release texture '%s'.", tempname);
	}
	else {
		// if the handle is new
		if (texref.handle == YUKI_INVALID_ID_U32) {
			// find the available slot 
			for (u32 i = 0; i < state_ref->config.max_textures_count; ++i) {
				if (state_ref->registered_textures[i].id == YUKI_INVALID_ID_U32) {
					*texture_id_ref = texref.handle = i;
					break;
				}
			}

			// not avaiable slot found, bail
			if (*texture_id_ref) {
				YUKI_LOG_CRITICAL("texture module cannot hold anymore textures - configuration adjustment is required!");
				return false;
			}

			// create new texture ...
			{
				yuki_texture *tex = &state_ref->registered_textures[texref.handle];
				if (!_ykstatic_texture_load(tex, texture_name)) {
					*texture_id_ref = YUKI_INVALID_ID_U32;
					YUKI_LOG_ERROR("failed to load texture '%s'", texture_name);
					return false;
				}
				tex->id = texref.handle;
				YUKI_LOG_VERBOSE("created texture '%s', now ref_count is %u.", texture_name, texref.ref_count);
			}
		}
		else {
			*texture_id_ref = texref.handle;
			YUKI_LOG_VERBOSE("texture '%s' already exists, ref_count increased to %u.", texture_name, texref.ref_count);;
		}
	}

	hash_table_set(&state_ref->registered_texture_table, tempname, &texref);
	return true;
}


bool
texture_module_startup
(u64 *required_memory_size, void *state, const yuki_texture_module_config config)
{
	if (config.max_textures_count == 0) {
		YUKI_LOG_ERROR("max texture count from configuration must be a non-zero positive value!");
		return false;
	}

	u64	modulesize = sizeof(yuki_texture_module_state),
		arrsize = sizeof(yuki_texture) * config.max_textures_count,
		htablesize = sizeof(yuki_texture_reference) * config.max_textures_count;

	*required_memory_size = modulesize + arrsize + htablesize;

	if (!state)
		return false;

	state_ref = YUKI_CAST(yuki_texture_module_state *, state);
	state_ref->config = config;

	void *arrblock = state_ref + modulesize;
	state_ref->registered_textures = arrblock;

	void *htblock = state_ref + modulesize + arrsize;
	hash_table_construct(&state_ref->registered_texture_table, sizeof(yuki_texture_reference), config.max_textures_count, htblock);

	// fill hash table with invalid references for default
	yuki_texture_reference invalidref;
	invalidref.handle = YUKI_INVALID_ID_U32;
	invalidref.ref_count = 0;
	hash_table_fill(&state_ref->registered_texture_table, &invalidref);

	// invalidate all textures in array
	for (u32 i = 0; i < config.max_textures_count; ++i)
		state_ref->registered_textures[i].id = YUKI_INVALID_ID_U32;

	return true;
}

bool
texture_module_shutdown
(void *state)
{
	if (!state)
		return false;

	hash_table_destruct(&state_ref->registered_texture_table);
	state_ref = NULL;

	return true;
}


yuki_texture *
texture_module_acquire_texture
(const_str name)
{
	if (!name) {
		YUKI_LOG_ERROR("given name is null - returning nullptr texture!");
		return NULL;
	}

	u32 id = YUKI_INVALID_ID_U32;
	if (!_ykstatic_process_texture_ref(&id, name, 1, true)) {
		YUKI_LOG_ERROR("failed to obtain new texture id!");
		return NULL;
	}

	return &state_ref->registered_textures[id];
}

bool
texture_module_release_texture
(const_str name)
{
	if (!name) {
		YUKI_LOG_ERROR("given name is null - returning nullptr texture!");
		return false;
	}

	u32 id = YUKI_INVALID_ID_U32;
	if (!_ykstatic_process_texture_ref(&id, name, -1, false)) {
		YUKI_LOG_ERROR("failed to release texture '%s'!", name);
	}

	return true;
}


