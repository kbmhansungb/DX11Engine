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

/// <summary>
/// Light 컴포넌트로 기본형은 SpotLight임.
/// Light의 포지션과 방향은 Owner의 트런세픔에 영향을 받음.
/// 21.11.15
/// </summary>
class Light : public Component, public Lighting
{

#pragma region ConDestruct

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

#pragma endregion

	// 뎁스 스텐실 쉐도우를 위한 버퍼
	// 21.11.15
#pragma region Val_DepthStencil

#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: 뎁스 스텐실을 버퍼로 넘길 수 있다면 보다 단순하게 만들 수 있음")
public:
	TEXTURE::RenderTargetTexture rtt_depth;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depth_stencil_buf;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depth_stencil_view;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> depth_srv;

	ConstantBuffer<BTYPE::CB_Light> light;
	ConstantBuffer<BTYPE::CB_ViewProjection> view_projection;
	std::list<SafePtr<Scene>> lighting_scenes;

#pragma endregion

	// 라이팅 규칙을 결정하는 쉐이더
	// 21.11.15
#pragma region LightShader

public:
	SafePtr<SHADER::MeshShader> default_depth_light_shader();
	SafePtr<SHADER::MeshShader> default_lighting_shader();

#pragma endregion

	// 뎁스 스텐실 쉐도우와 관련된 함수
	// 21.11.15
#pragma region Func_DepthStencil

public:
	/// <summary>
	/// 뎁스 스텐실 변수를 생성하는 메서드
	/// 21.11.15
	/// </summary>
	void create_depth_stencil();

	/// <summary>
	/// 델리게이트 바인드 메서드
	/// Owner의 포지션이 변경되었을 때 
	/// 라이트 포즈를 변경하는 메서드
	/// 21.11.15
	/// </summary>
	/// <param name="object"></param>
	void update_pos(SafePtr<GameObject> object);

#pragma endregion

	// 라이팅을 위한 함수
	// 21.11.15
#pragma region Func_Light

public:
	/// <summary>
	/// 뎁스 스텐실 업데이트
	/// 21.11.15
	/// </summary>
	/// <param name="sub_graphics"></param>
	virtual void update_stencil(SubGraphics* sub_graphics);

	/// <summary>
	/// 라이팅 메서드, 디퍼드 렌더링이 Present될 때 호출됨
	/// 라이트 지오메트리 버퍼에 라이트 웨이트를 더함
	/// 21.11.15
	/// </summary>
	/// <param name="sub_graphics"></param>
	virtual void lighting(SubGraphics* sub_graphics) override;

#pragma endregion

#pragma region Component

public:
	virtual void awake() override;
	virtual void sleep() override;
	virtual void draw_detail_view() override;

#pragma endregion
};
REGIST_COMPONENT(Light);