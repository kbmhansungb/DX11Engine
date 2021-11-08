#pragma once

#include "Engine.h"
#include "Resource/_Exclude.h"
#include "ResourceImporter.h"

namespace RESOURCE_IMPORT_SYSTEM
{
	class Image_import : public ResourceImporter
	{
	public:
		Image_import() : ResourceImporter("Texture") {}

		virtual void import_file(Engine* engine, std::string path) override;
	};
	static Image_import image_import_instance;
}