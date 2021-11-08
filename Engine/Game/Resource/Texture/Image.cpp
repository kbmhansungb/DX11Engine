#include "Image.h"
#include "../../Engine.h"

void TEXTURE::Image::load_resource(Engine* engine)
{
	Texture::load_resource(engine);
	try
	{
		if (StringHelper::GetFileExtension(source_path) == "dds")
		{
			HRESULT hr = CreateDDSTextureFromFile(engine->Device.Get(), StringHelper::StringToWide(this->source_path).c_str(), this->texture.GetAddressOf(), this->textureView.GetAddressOf());
			COM_ERROR_IF_FAILED(hr, "Failed to load Texture from file.");
		}
		else
		{
			HRESULT hr = CreateWICTextureFromFile(engine->Device.Get(), StringHelper::StringToWide(this->source_path).c_str(), this->texture.GetAddressOf(), this->textureView.GetAddressOf());
			COM_ERROR_IF_FAILED(hr, "Failed to load Texture from file.");
		}
	}
	catch (COMException& exp)
	{
		ErrorLogger::Log(exp);

		int w = 1, h = 1;
		const Color* color = &texColors::UnloadedTextureColor;

		try
		{
			CD3D11_TEXTURE2D_DESC texDesc;
			texDesc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_R8G8B8A8_UNORM, w, h);

			ID3D11Texture2D* p2DTex = nullptr;
			D3D11_SUBRESOURCE_DATA initialData{};
			initialData.pSysMem = color;
			initialData.SysMemPitch = w * sizeof(Color);
			HRESULT hr = engine->Device.Get()->CreateTexture2D(&texDesc, &initialData, &p2DTex);
			COM_ERROR_IF_FAILED(hr, "Failed to create texture2d from color data.");
			this->texture = static_cast<ID3D11Texture2D*>(p2DTex);
			CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, texDesc.Format);
			hr = engine->Device.Get()->CreateShaderResourceView(this->texture.Get(), &srvDesc, this->textureView.GetAddressOf());
			COM_ERROR_IF_FAILED(hr, "Failed to create shader resourceview from texture generate from color data.");
		}
		catch (COMException& exp)
		{
			ErrorLogger::Log(exp);
			return;
		}
		return;
	}
	return;
}

void TEXTURE::Image::draw_detail_view()
{
	Texture::draw_detail_view();

	if (ImGui::CollapsingHeader("Image"))
	{

	}
}
