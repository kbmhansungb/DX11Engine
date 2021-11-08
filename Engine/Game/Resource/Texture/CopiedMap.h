#pragma once

#include "_Texture.h"

namespace TEXTURE
{
	class CopiedImage : public Texture
	{
	protected:
		const uint8_t* pData;
		size_t size;

	public:
		CopiedImage(
			std::string source_path,
			const uint8_t* pData, 
			size_t size
		);
		virtual void load_resource(Engine* engine) override;

		virtual void draw_detail_view() override;
	};
}