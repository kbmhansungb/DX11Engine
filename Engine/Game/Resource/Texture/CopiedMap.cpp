#include "CopiedMap.h"
#include "../../Engine.h"

using namespace std;

TEXTURE::CopiedImage::CopiedImage(string source_path, const uint8_t* pData, size_t size) :
	TEXTURE::Texture(source_path),
	pData(pData), size(size)
{
}

void TEXTURE::CopiedImage::load_resource(Engine* engine)
{
	Texture::load_resource(engine);
	try
	{
		HRESULT hr;
		hr = CreateWICTextureFromMemory(engine->Device.Get(), pData, size, this->texture.GetAddressOf(), this->textureView.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create texture from memory.");
	}
	catch (COMException& exp)
	{
		ErrorLogger::Log(exp);
	}
}

void TEXTURE::CopiedImage::draw_detail_view()
{
	Texture::draw_detail_view();

	if (ImGui::CollapsingHeader("Copied image"))
	{

	}
}
