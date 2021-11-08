#include "DefaultResourceImporter.h"

void RESOURCE_IMPORT_SYSTEM::Image_import::import_file(Engine* engine, std::string path)
{
	NAME::Name texture_name = Name(
		&engine->name_container,
		path
	);

	SafePtr<TEXTURE::Texture> texture
		= engine->resources.get_resource(texture_name)
		.cast<TEXTURE::Texture>();

	if (texture.is_vaild() == false)
	{
		std::unique_ptr<TEXTURE::Image> texture_uptr =
			std::make_unique<TEXTURE::Image>(path);

		engine->resources.add_resource(texture_name, texture_uptr);
	}
}
