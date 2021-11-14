#include "RenderToTexture.h"
#include "../Graphics/_SubGraphics.h"


RenderToTexture::RenderToTexture(UINT resource_width, UINT resource_height) :
	width(resource_width),
	height(resource_height),
	depth_stencil_desc(DXGI_FORMAT_D24_UNORM_S8_UINT, resource_width, resource_height)
{
	depth_stencil_desc.MipLevels = 1;
	depth_stencil_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
}

void RenderToTexture::awake()
{
	Engine* engine = get_owner()->this_scene->this_engine.get();
	ID3D11Device* device = engine->Device.Get();

	render_target_texture_0 = std::make_unique<TEXTURE::RenderTargetTexture>(width, height);
	render_target_texture_1 = std::make_unique<TEXTURE::RenderTargetTexture>(width, height);
	render_target_texture_2 = std::make_unique<TEXTURE::RenderTargetTexture>(width, height);
	render_target_texture_3 = std::make_unique<TEXTURE::RenderTargetTexture>(width, height);
	render_target_texture_4 = std::make_unique<TEXTURE::RenderTargetTexture>(width, height);
	render_target_texture_5 = std::make_unique<TEXTURE::RenderTargetTexture>(width, height);
	render_target_texture_6 = std::make_unique<TEXTURE::RenderTargetTexture>(width, height);
	render_target_texture_7 = std::make_unique<TEXTURE::RenderTargetTexture>(width, height);

	render_target_textures.resize(8);
	render_target_textures[0] = render_target_texture_0.get();
	render_target_textures[1] = render_target_texture_1.get();
	render_target_textures[2] = render_target_texture_2.get();
	render_target_textures[3] = render_target_texture_3.get();
	render_target_textures[4] = render_target_texture_4.get();
	render_target_textures[5] = render_target_texture_5.get();
	render_target_textures[6] = render_target_texture_6.get();
	render_target_textures[7] = render_target_texture_7.get();

	render_target_views.resize(render_target_textures.size());
	for (SIZE_T index = 0; index < render_target_textures.size(); ++index)
	{
		render_target_textures[index]->load_resource(engine);
		render_target_views[index] = render_target_textures[index]->render_target_view.Get();
	}

	device->CreateTexture2D(&depth_stencil_desc, 0, this->depth_stencil_texture.GetAddressOf());
	device->CreateDepthStencilView(depth_stencil_texture.Get(), 0, depth_stencil_view.GetAddressOf());
}

void RenderToTexture::set_render_target(SubGraphics* sub_graphics)
{
	// viewport 설정
	CD3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0.f;
	viewport.TopLeftY = 0.f;
	viewport.Width = this->width;
	viewport.Height = this->height;
	viewport.MinDepth = D3D11_MIN_DEPTH;
	viewport.MaxDepth = D3D11_MAX_DEPTH;

	CD3D11_VIEWPORT viewport2;
	viewport2.TopLeftX = 0.f;
	viewport2.TopLeftY = 0.f;
	viewport2.Width = this->width / 2;
	viewport2.Height = this->height / 2;
	viewport2.MinDepth = D3D11_MIN_DEPTH;
	viewport2.MaxDepth = D3D11_MAX_DEPTH;

	CD3D11_VIEWPORT viewports[8] = {
		viewport,
		viewport,
		viewport,
		viewport2,
		viewport,
		viewport,
		viewport,
		viewport
	};

	sub_graphics->Device_context->RSSetViewports(8, viewports);

	sub_graphics->Device_context->OMSetRenderTargets(render_target_views.size(), render_target_views.data(), depth_stencil_view.Get());
}

void RenderToTexture::ClearRenderTarget(SubGraphics* sub_graphics)
{
	for (auto& views : render_target_views)
	{
		sub_graphics->Device_context->ClearRenderTargetView(views, &Clear_color.x);
	}
	sub_graphics->Device_context->ClearDepthStencilView(depth_stencil_view.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void RenderToTexture::resize_render_texture(UINT width, UINT height)
{
	if (width < 1 && height < 1) return;
	this->width = width;
	this->height = height;

	//////

	Engine* engine = get_owner()->this_scene->this_engine.get();
	ID3D11Device* device = engine->Device.Get();

	depth_stencil_texture.Reset();
	depth_stencil_view.Reset();

	depth_stencil_desc.Width = width;
	depth_stencil_desc.Height = height;

	device->CreateTexture2D(&depth_stencil_desc, 0, this->depth_stencil_texture.GetAddressOf());
	device->CreateDepthStencilView(depth_stencil_texture.Get(), 0, depth_stencil_view.GetAddressOf());

	render_target_texture_0->texture_desc.Width = width;
	render_target_texture_1->texture_desc.Width = width;
	render_target_texture_2->texture_desc.Width = width;
	render_target_texture_3->texture_desc.Width = width;
	render_target_texture_4->texture_desc.Width = width;
	render_target_texture_5->texture_desc.Width = width;
	render_target_texture_6->texture_desc.Width = width;
	render_target_texture_7->texture_desc.Width = width;

	render_target_texture_0->texture_desc.Height = height;
	render_target_texture_1->texture_desc.Height = height;
	render_target_texture_2->texture_desc.Height = height;
	render_target_texture_3->texture_desc.Height = height;
	render_target_texture_4->texture_desc.Height = height;
	render_target_texture_5->texture_desc.Height = height;
	render_target_texture_6->texture_desc.Height = height;
	render_target_texture_7->texture_desc.Height = height;

	render_target_views.resize(render_target_textures.size());
	for (SIZE_T index = 0; index < render_target_textures.size(); ++index)
	{
		render_target_textures[index]->load_resource(engine);
		render_target_views[index] = render_target_textures[index]->render_target_view.Get();
	}
}

void RenderToTexture::set_render_texture(SubGraphics* sub_graphics)
{
	ID3D11ShaderResourceView* resource_view[] = {
		render_target_textures[0]->GetTextureResourceView(),
		render_target_textures[1]->GetTextureResourceView(),
		render_target_textures[2]->GetTextureResourceView(),
		render_target_textures[3]->GetTextureResourceView(),
		render_target_textures[4]->GetTextureResourceView(),
		render_target_textures[5]->GetTextureResourceView(),
		render_target_textures[6]->GetTextureResourceView(),
		render_target_textures[7]->GetTextureResourceView(),
	};
	ID3D11SamplerState* sampler[] = {
		render_target_textures[0]->GetSamplerState(),
		render_target_textures[1]->GetSamplerState(),
		render_target_textures[2]->GetSamplerState(),
		render_target_textures[3]->GetSamplerState(),
		render_target_textures[4]->GetSamplerState(),
		render_target_textures[5]->GetSamplerState(),
		render_target_textures[6]->GetSamplerState(),
		render_target_textures[7]->GetSamplerState(),
	};

	sub_graphics->Device_context->PSSetShaderResources(
		0,
		8,
		resource_view);
	sub_graphics->Device_context->PSSetSamplers(
		0,
		8,
		sampler);
}

void RenderToTexture::draw_detail_view()
{
	RenderTarget::draw_detail_view();

	if (ImGui::CollapsingHeader("Render to texture"))
	{
#pragma message(__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: 렌더투 텍스쳐 개선후에 ImGui 필드를 만들어야 함.")
	}
}

RenderToDeferred::RenderToDeferred(int resource_width, int resource_height) :
	RenderToTexture(resource_width, resource_height)
{
}

#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: 임시로 여기다 구현함")
#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: 텍스쳐로 옮겨야 하지 않을까?")
#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: 근데 지금 텍스쳐 구조가 쓰래기 같아서 안됨..")
void RenderToDeferred::copy_render_texture(
	SubGraphics* sub_graphics,
	Microsoft::WRL::ComPtr<ID3D11Texture2D>& texture_2d,
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shader_resource_view,
	Microsoft::WRL::ComPtr<ID3D11SamplerState>& sampler_state,
	TEXTURE::RenderTargetTexture* render_target_texture
)
{
	ID3D11Texture2D* t2d = dynamic_cast<ID3D11Texture2D*>(render_target_texture->texture_2d.Get());
	D3D11_TEXTURE2D_DESC texture_desc;
	t2d->GetDesc(&texture_desc);
	sub_graphics->Device->CreateTexture2D(&texture_desc, NULL, texture_2d.GetAddressOf());
	sub_graphics->Device_context->CopyResource(
		texture_2d.Get(),
		t2d
	);
	CD3D11_SHADER_RESOURCE_VIEW_DESC srv_desc
		= CD3D11_SHADER_RESOURCE_VIEW_DESC(
			texture_2d.Get(),
			D3D11_SRV_DIMENSION::D3D10_1_SRV_DIMENSION_TEXTURE2D
		);
	sub_graphics->Device->CreateShaderResourceView(
		texture_2d.Get(),
		&srv_desc,
		shader_resource_view.GetAddressOf()
	);
	D3D11_SAMPLER_DESC sampler_desc = CD3D11_SAMPLER_DESC(CD3D11_DEFAULT());
	sub_graphics->Device->CreateSamplerState(
		&sampler_desc,
		sampler_state.GetAddressOf()
	);
}

void RenderToDeferred::Present(SubGraphics* sub_graphics)
{
	RenderToTexture::Present(sub_graphics);
	
	for (auto& scene : sub_graphics->Drawing_scenes)
	{
		if (scene.is_vaild())
		{
			for (const auto& light : scene->get_lighting_uor_set())
			{
				light->update_stencil(sub_graphics);
			}
		}
	}

	this->set_render_target(sub_graphics);

	// world position을 copy해서 넘겨야함.
	Microsoft::WRL::ComPtr<ID3D11Texture2D> wp_t2d, no_t2d;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> wp_srv, no_srv;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> wp_ss, no_ss;
	copy_render_texture(sub_graphics, wp_t2d, wp_srv, wp_ss, render_target_texture_4.get());
	copy_render_texture(sub_graphics, no_t2d, no_srv, no_ss, render_target_texture_1.get());
	sub_graphics->Device_context->PSSetShaderResources(
		2, 1, wp_srv.GetAddressOf()
	);
	sub_graphics->Device_context->PSSetSamplers(
		2, 1, wp_ss.GetAddressOf()
	);
	sub_graphics->Device_context->PSSetShaderResources(
		3, 1, no_srv.GetAddressOf()
	);
	sub_graphics->Device_context->PSSetSamplers(
		3, 1, no_ss.GetAddressOf()
	);

	// lighting을 그리는 구간.
	for (auto& scene : sub_graphics->Drawing_scenes)
	{
		if (scene.is_vaild())
		{
			for (const auto& light : scene->get_lighting_uor_set())
			{
				light->lighting(sub_graphics);
			}
		}
	}

	SafePtr<MESH::SpriteMesh> mesh = MESH::default_sprite_mesh(this->owner->this_scene->this_engine.get());
	SpritePipe::set_sprite_vs(sub_graphics);
	mesh->set_sprite_mesh(sub_graphics);
	for (auto& dp : deferred_processes)
	{
		if (dp.is_vaild())
		{
			dp->deferred_process(this, sub_graphics);
			mesh->draw_sprite_mesh(sub_graphics);
		}
	}
}

void RenderToDeferred::draw_detail_view()
{
	RenderToTexture::draw_detail_view();

	if (ImGui::CollapsingHeader("Render to deferred"))
	{
		ImGui::list_field_default(deferred_processes, "Deferred processes",
			[=](SafePtr<DeferredProcess>& dp) 
			{
				ImGui::base_field(dp);
			});
	}
}

void DeferredProcess::deferred_process(RenderToDeferred* rtd, SubGraphics* sub_graphics)
{
	this->set_mesh_shader(sub_graphics);
}
