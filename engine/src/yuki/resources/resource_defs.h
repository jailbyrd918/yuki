#ifndef YUKI_HEADER_RESOURCE_DEFS
#define YUKI_HEADER_RESOURCE_DEFS

#include "yuki/math/math_defs.h"
#include "yuki/renderer/renderer_defs.h"


#define	YUKI_MESH_NAME_MAX_LENGTH	256
#define	YUKI_TEXTURE_NAME_MAX_LENGTH	512


typedef enum e_yuki_resource_type {

	YUKI_RESOURCE_TYPE_IMAGE,
	YUKI_RESOURCE_TYPE_MESH,

	YUKI_RESOURCE_TYPES_NUM

}
yuki_resource_type;

typedef struct s_yuki_resource {

	u32		loader_id;

	const_str	name;
	const_str	filepath;

	u64		data_size;
	void		*data;

}
yuki_resource;

typedef struct s_yuki_image_resource_data {

	s32		width, height;
	color_buffer	color_buffer;

}
yuki_image_resource_data;

typedef struct s_yuki_texture {

	u32	id;
	char	name[YUKI_TEXTURE_NAME_MAX_LENGTH];

	u32	width, height;

	void	*data;

}
yuki_texture;

typedef struct s_yuki_mesh_vertex_index_data {

	s32	position_index,
		normal_index,
		texture_index;

}
yuki_mesh_vertex_index_data;

typedef struct s_yuki_mesh_face_data {

	yuki_mesh_vertex_index_data	vertices[3];

}
yuki_mesh_face_data;

typedef struct s_yuki_mesh_resource_data {

	yuki_mesh_face_data	*faces;
	u32			faces_count;

	yuki_vec2		*uv_coordinates;
	u32			uv_coordinates_count;

	yuki_vec3		*vertices;
	u32			vertices_count;

}
yuki_mesh_resource_data;

typedef struct s_yuki_mesh {

	u32	id;
	char	name[YUKI_MESH_NAME_MAX_LENGTH];
	char	texture_name[YUKI_TEXTURE_NAME_MAX_LENGTH];

	void	*data;

}
yuki_mesh;

typedef struct s_yuki_resource_module_config {

	str	asset_base_path;
	u32	register_loaders_count;

}
yuki_resource_module_config;

typedef struct s_yuki_resource_loader {

	u32			id;
	yuki_resource_type	resrc_type;
	
	str			type_directory;

	bool(*pfn_load)(struct s_yuki_resource_loader *self, const_str name, yuki_resource *resource_ref);
	bool(*pfn_unload)(struct s_yuki_resource_loader *self, yuki_resource *resource_ref);

}
yuki_resource_loader;



#endif
