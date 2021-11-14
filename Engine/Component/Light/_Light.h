#pragma once

#include "../_Include.h"
#include "../Renderer/_ViaRenderer.h"
#include "../../Game/Pipeline/SpritePipe/SpritePipe.h"

namespace BTYPE {
	struct CB_Light
	{
		XMFLOAT3 light_color = { 1.0f, 1.0f, 1.0f };
		float light_strength = 1.0f;
		XMVECTOR light_vec{};
		XMFLOAT3 light_pos{};
		float a = 1.f;
		float b = 0.f;
		XMFLOAT3 ambient_color{};
		float ambient_strength = 0.f;

		static UINT T_get_constant_buffer_slot() { return 10; }
		static bool T_use_in_vs() { return false; }
		static bool T_use_in_ps() { return true; }
	};
}

class Light : public Component, public Lighting
{
public:
	Light() : rtt_depth(100.f, 100.f)
	{
		rtt_depth.sampler_desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
		rtt_depth.sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		rtt_depth.sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		rtt_depth.sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		rtt_depth.sampler_desc.ComparisonFunc = D3D11_COMPARISON_LESS;
		rtt_depth.sampler_desc.BorderColor[0] = 0;
		rtt_depth.sampler_desc.BorderColor[1] = 0;
		rtt_depth.sampler_desc.BorderColor[2] = 0;
		rtt_depth.sampler_desc.BorderColor[3] = 0;
		rtt_depth.sampler_desc.MinLOD = 0;
		rtt_depth.sampler_desc.MaxLOD = 0;// D3D11_FLOAT32_MAX;
	}

	// light stencil
	TEXTURE::RenderTargetTexture rtt_depth;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depth_stencil_buf;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depth_stencil_view;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> depth_srv;

	ConstantBuffer<BTYPE::CB_Light> light;
	ConstantBuffer<BTYPE::CB_ViewProjection> view_projection;
	std::list<SafePtr<Scene>> lighting_scenes;

	virtual void awake() override;
	virtual void sleep() override;
	virtual void update() override;
	virtual void update_stencil(SubGraphics* sub_graphics);
	virtual void lighting(SubGraphics* sub_graphics) override;
	SafePtr<SHADER::MeshShader> default_depth_light_shader();
	SafePtr<SHADER::MeshShader> default_lighting_shader();
	virtual void draw_detail_view() override;

	void update_pos();

	void create_depth_stencil();
};
REGIST_COMPONENT(Light);