#pragma once
#include "_Texture.h"

namespace TEXTURE
{
	class Image : public Texture
	{
	public:
		Image(std::string source_path) : Texture(source_path) {}
		virtual void load_resource(Engine* engine) override;
		virtual void draw_detail_view() override;
	};
}

//namespace RESOURCE_IMPORT_SYSTEM
//{
//	class Image_import : public ResourceImporter
//	{
//	public:
//		Image_import() : ResourceImporter("Texture importer") {}
//
//		virtual void import_file(Engine* engine, std::string path) override;
//	};
//	static Image_import image_import_instance;
//}