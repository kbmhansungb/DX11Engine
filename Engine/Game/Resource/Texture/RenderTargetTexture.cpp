#include "RenderTargetTexture.h"
#include "../../Engine.h"

TEXTURE::RenderTargetTexture::RenderTargetTexture(UINT resource_width, UINT resource_height) :
	Texture(""),
	texture_desc(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, resource_width, resource_height),
	shader_resource_view_desc(D3D11_SRV_DIMENSION_TEXTURE2D, texture_desc.Format),
	render_target_view_desc(D3D11_RTV_DIMENSION_TEXTURE2D, texture_desc.Format)
{
	texture_desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	//texture_desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
	shader_resource_view_desc.Texture2D.MipLevels = 1;
}

void TEXTURE::RenderTargetTexture::resize_texture(float width, float height, Engine* engine)
{
	texture_desc.Width = width;
	texture_desc.Height = height;

	load_resource(engine);
}

void TEXTURE::RenderTargetTexture::release_resource(Engine* engine)
{
	Texture::release_resource(engine);

	//texture_2d.Reset();
	render_target_view.Reset();
}

void TEXTURE::RenderTargetTexture::load_resource(Engine* engine)
{
	release_resource(engine);
	Texture::load_resource(engine);

	//ID3D11Texture2D* texture_2d = nullptr;
	engine->Device->CreateTexture2D(&texture_desc, NULL, texture_2d.GetAddressOf());
	//this->texture = texture_2d;
	engine->Device->CreateShaderResourceView(texture_2d.Get(), &shader_resource_view_desc, textureView.GetAddressOf());
	engine->Device->CreateRenderTargetView(texture_2d.Get(), &render_target_view_desc, render_target_view.GetAddressOf());
}

void TEXTURE::RenderTargetTexture::draw_detail_view()
{
	Texture::draw_detail_view();

	if (ImGui::CollapsingHeader("Render target texture"))
	{

	}
}
