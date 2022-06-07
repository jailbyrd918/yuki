#include "yuki/core/debug_log.h"
#include "yuki/core/memory/memory_module.h"
#include "yuki/core/str.h"
#include "yuki/platform/filesystem.h"
#include "yuki/resources/resource_module.h"

#include "yuki/resources/loaders/image_loader.h"

#include <stdlib.h>
#include <SDL2/SDL_image.h>


static const_str extensions[] = {
	".jpg",
	".jpeg",
	".png",
	".tif",
	".tiff",
	".webp"
};


static bool
_ykstatic_image_loader_load
(struct s_yuki_resource_loader *self, const_str name, yuki_resource *resource_ref)
{
	if (!self || !name || !resource_ref)
		return false;

	// format: [asset_base_filepath]/[type_directory]/[name][extension]
	const_str fmtstr = "%s/%s/%s%s";
	
	char	filepath[512];
	bool	imagefound = false;

	u32	numextensions = sizeof(extensions) / sizeof(*extensions);
	for (u32 i = 0; i < numextensions; ++i) {
		str_format_string(filepath, fmtstr, "assets", self->type_directory, name, extensions[i]);
		
		if (filesystem_check_file_exists(filepath)) {
			imagefound = true;
			break;
		}
	}

	if (!imagefound) {
		YUKI_LOG_ERROR("failed to locate image of filepath '%s' with any supported extension!", filepath);
		return false;
	}

	yuki_image_resource_data *data = memory_module_allocate_block(sizeof(yuki_image_resource_data), YUKI_MEMORY_TAG_TEXTURE);

	// load image ... 
	{
		SDL_Surface *img = IMG_Load(filepath);
		if (!img) {
			YUKI_LOG_ERROR("failed to load image file from path '%s'!", filepath);
			return false;
		}

		data->width = img->w;
		data->height = img->h;

		data->color_buffer = memory_module_allocate_block(sizeof(color32) * (img->w * img->h), YUKI_MEMORY_TAG_TEXTURE);
		memory_module_copy_block(data->color_buffer, img->pixels, sizeof(color32) * (img->w * img->h));
	
		SDL_FreeSurface(img);
	}

	resource_ref->data = data;
	resource_ref->data_size = sizeof(yuki_image_resource_data);
	resource_ref->name = name;

	return true;
}

static bool
_ykstatic_image_loader_unload
(struct s_yuki_resource_loader *self, yuki_resource *resource_ref)
{
	if (!self || !resource_ref) {
		YUKI_LOG_ERROR("self reference to loader or reference to resource is null!");
		return false;
	}
	else if (!resource_ref->data) {
		YUKI_LOG_ERROR("reference to resource data is null!");
		return false;
	}

	// deallocate
	{
		yuki_image_resource_data *resrcdata = YUKI_CAST(yuki_image_resource_data *, resource_ref->data);
		memory_module_deallocate_block(resrcdata->color_buffer, sizeof(color32) * (resrcdata->width * resrcdata->height), YUKI_MEMORY_TAG_TEXTURE);
		memory_module_deallocate_block(resrcdata, sizeof(yuki_image_resource_data), YUKI_MEMORY_TAG_TEXTURE);
	}

	return true;
}


yuki_resource_loader
resouce_image_loader_construct
(void)
{
	yuki_resource_loader loader;
	loader.resrc_type = YUKI_RESOURCE_TYPE_IMAGE;
	loader.type_directory = "images";
	loader.pfn_load = _ykstatic_image_loader_load;
	loader.pfn_unload = _ykstatic_image_loader_unload;
	
	return loader;
}



