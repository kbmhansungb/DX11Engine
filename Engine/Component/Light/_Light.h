#pragma once

#include "../_Include.h"
#include "../Renderer/_ViaRenderer.h"
#include "../../Game/Pipeline/SpritePipe/SpritePipe.h"

namespace BTYPE {
	struct CB_LIGHT_POS
	{
		DirectX::XMFLOAT3 light_pos{};
		DirectX::XMVECTOR dir{};
		float resource;

		static UINT T_get_constant_buffer_slot() { return 11; }
		static bool T_use_in_vs() { return false; }
		static bool T_use_in_ps() { return true; }
	};
	struct CB_Light
	{
		DirectX::XMFLOAT3 light_color = { 1.0f, 1.0f, 1.0f };
		float light_strength = 1.0f;

		static UINT T_get_constant_buffer_slot() { return 10; }
		static bool T_use_in_vs() { return false; }
		static bool T_use_in_ps() { return true; }
	};
}

class Light : public Component, public Lighting
{
public:
	Light() : rtt_depth(100.f, 100.f), rtt_LightColor(100.f, 100.f)
	{
		rtt_depth.sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		rtt_depth.sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		rtt_depth.sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		rtt_depth.sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		rtt_depth.sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		rtt_depth.sampler_desc.BorderColor[0] = 0;
		rtt_depth.sampler_desc.BorderColor[1] = 0;
		rtt_depth.sampler_desc.BorderColor[2] = 0;
		rtt_depth.sampler_desc.BorderColor[3] = 0;
		rtt_depth.sampler_desc.MinLOD = 0;
		rtt_depth.sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

		rtt_LightColor.sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		rtt_LightColor.sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		rtt_LightColor.sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		rtt_LightColor.sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		rtt_LightColor.sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		rtt_LightColor.sampler_desc.BorderColor[0] = 0;
		rtt_LightColor.sampler_desc.BorderColor[1] = 0;
		rtt_LightColor.sampler_desc.BorderColor[2] = 0;
		rtt_LightColor.sampler_desc.BorderColor[3] = 0;
		rtt_LightColor.sampler_desc.MinLOD = 0;
		rtt_LightColor.sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

		light_pos.data.resource = 100.f;
	}

	// light stencil
	TEXTURE::RenderTargetTexture rtt_depth;
	TEXTURE::RenderTargetTexture rtt_LightColor;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depth_stencil_buf;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depth_stencil_view;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> depth_srv;

	ConstantBuffer<BTYPE::CB_LIGHT_POS> light_pos;
	ConstantBuffer<BTYPE::CB_Light> light;
	ConstantBuffer<BTYPE::CB_ViewProjection> view_projection;
	std::list<SafePtr<Scene>> lighting_scenes;

	virtual void awake() override;
	virtual void sleep() override;
	virtual void update_stencil(SubGraphics* sub_graphics);
	virtual void lighting(SubGraphics* sub_graphics) override;
	SafePtr<SHADER::MeshShader> default_depth_light_shader();
	SafePtr<SHADER::MeshShader> default_lighting_shader();
	virtual void draw_detail_view() override;

	void create_depth_stencil();
};
REGIST_COMPONENT(Light);