#include "yuki/core/debug_log.h"
#include "yuki/core/memory/memory_module.h"
#include "yuki/core/str.h"
#include "yuki/containers/dynamic_array.h"
#include "yuki/resources/resource_module.h"

#include "yuki/resources/loaders/mesh_loader.h"

#include <stdio.h>


#define	MESH_FILE_EXTENSION	".obj"
#define	MESH_ENTRY_BUFFER_MAX	1024


static void
_ykstatic_mesh_loader_load_obj_from_file
(yuki_mesh_resource_data *data, yuki_file_handle *file_handle)
{
	FILE *infile = YUKI_CAST(FILE *, file_handle->ref);
	char line[MESH_ENTRY_BUFFER_MAX];

	while (fgets(line, MESH_ENTRY_BUFFER_MAX, infile)) {
		// parse vertex ...
		if (str_strings_equal_ranged_sensitive(line, "v ", 2)) {
			yuki_vec3 verx;
			sscanf(line, "v %f %f %f", &verx.x, &verx.y, &verx.y);
			dynamic_array_push_back(data->vertices, verx);
		}

		// parse texture coordinate ...
		if (str_strings_equal_ranged_sensitive(line, "vt ", 3)) {
			yuki_vec2 uv;
			sscanf(line, "vt %f %f", &uv.u, &uv.v);
			dynamic_array_push_back(data->uv_coordinates, uv);
		}

		// parse mesh face ...
		if (str_strings_equal_ranged_sensitive(line, "f ", 2)) {
			yuki_mesh_vertex_index_data indices[3];

			sscanf(
				line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
				&indices[0].position_index, &indices[0].texture_index, &indices[0].normal_index,
				&indices[1].position_index, &indices[1].texture_index, &indices[1].normal_index,
				&indices[2].position_index, &indices[2].texture_index, &indices[2].normal_index
			);

			yuki_mesh_face_data face;
			memory_module_copy_block(face.vertices, indices, sizeof(yuki_mesh_vertex_index_data) * 3);
			dynamic_array_push_back(data->faces, face);
		}
	}

	// initialize number of vertices, uv_coordinates and faces
	{
		data->vertices_count = dynamic_array_get_length(data->vertices);
		data->uv_coordinates_count = dynamic_array_get_length(data->uv_coordinates);
		data->faces_count = dynamic_array_get_length(data->faces);
	}
}

static bool
_ykstatic_mesh_loader_load
(struct s_yuki_resource_loader *self, const_str name, yuki_resource *resource_ref)
{
	if (!self || !name || !resource_ref) {
		YUKI_LOG_ERROR("reference to %s is null!", !self ? "loader" : (!name ? "name" : "resource"));
		return false;
	}

	// format: [assets_base_filepath]/[type_directory]/[name].obj
	const_str fmtstr = "%s/%s/%s%s";

	yuki_file_handle handle;
	char filepath[512] = { '\0' };

	str_format_string(filepath, fmtstr, resouce_module_get_base_filepath(), self->type_directory, name, MESH_FILE_EXTENSION);
	if (filesystem_check_file_exists(filepath)) {
		if (!filesystem_open_file(filepath, YUKI_FILE_IO_MODE_READ, YUKI_FILE_MODE_TEXT, &handle)) {
			YUKI_LOG_ERROR("failed to open file from path '%s'!", filepath);
			return false;
		}
	}

	yuki_mesh_resource_data *data = memory_module_allocate_block(sizeof(yuki_mesh_resource_data), YUKI_MEMORY_TAG_GEOMETRY);
	data->vertices = dynamic_array_alloc(yuki_vec3);
	data->uv_coordinates = dynamic_array_alloc(yuki_vec2);
	data->faces = dynamic_array_alloc_reserve_capacity(yuki_mesh_face_data, 20);
	_ykstatic_mesh_loader_load_obj_from_file(data, &handle);

	resource_ref->data = data;
	resource_ref->data_size = sizeof(yuki_mesh_resource_data);
	resource_ref->name = name;

	return true;
}

static bool
_ykstatic_mesh_loader_unload
(struct s_yuki_resource_loader *self, yuki_resource *resource_ref)
{
	if (!self || !resource_ref || !resource_ref->data) {
		YUKI_LOG_ERROR("reference to %s is null!", !self ? "loader self" : (!resource_ref ? "resource" : "resource data"));
		return false;
	}

	// deallocate
	{
		yuki_mesh_resource_data *resrcdata = YUKI_CAST(yuki_mesh_resource_data *, resource_ref->data);
		dynamic_array_free(resrcdata->faces);
		dynamic_array_free(resrcdata->uv_coordinates);
		dynamic_array_free(resrcdata->vertices);
		memory_module_deallocate_block(resrcdata, sizeof(yuki_mesh_resource_data), YUKI_MEMORY_TAG_GEOMETRY);
	}

	return true;
}


yuki_resource_loader
resource_mesh_loader_construct
(void)
{
	yuki_resource_loader loader;
	loader.resrc_type = YUKI_RESOURCE_TYPE_MESH;
	loader.type_directory = "meshes";
	loader.pfn_load = _ykstatic_mesh_loader_load;
	loader.pfn_unload = _ykstatic_mesh_loader_unload;

	return loader;
}






