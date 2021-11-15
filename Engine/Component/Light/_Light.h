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
/// Light ������Ʈ�� �⺻���� SpotLight��.
/// Light�� �����ǰ� ������ Owner�� Ʈ�����Ŀ� ������ ����.
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

	// ���� ���ٽ� �����츦 ���� ����
	// 21.11.15
#pragma region Val_DepthStencil

#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: ���� ���ٽ��� ���۷� �ѱ� �� �ִٸ� ���� �ܼ��ϰ� ���� �� ����")
public:
	TEXTURE::RenderTargetTexture rtt_depth;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depth_stencil_buf;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depth_stencil_view;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> depth_srv;

	ConstantBuffer<BTYPE::CB_Light> light;
	ConstantBuffer<BTYPE::CB_ViewProjection> view_projection;
	std::list<SafePtr<Scene>> lighting_scenes;

#pragma endregion

	// ������ ��Ģ�� �����ϴ� ���̴�
	// 21.11.15
#pragma region LightShader

public:
	SafePtr<SHADER::MeshShader> default_depth_light_shader();
	SafePtr<SHADER::MeshShader> default_lighting_shader();

#pragma endregion

	// ���� ���ٽ� ������� ���õ� �Լ�
	// 21.11.15
#pragma region Func_DepthStencil

public:
	/// <summary>
	/// ���� ���ٽ� ������ �����ϴ� �޼���
	/// 21.11.15
	/// </summary>
	void create_depth_stencil();

	/// <summary>
	/// ��������Ʈ ���ε� �޼���
	/// Owner�� �������� ����Ǿ��� �� 
	/// ����Ʈ ��� �����ϴ� �޼���
	/// 21.11.15
	/// </summary>
	/// <param name="object"></param>
	void update_pos(SafePtr<GameObject> object);

#pragma endregion

	// �������� ���� �Լ�
	// 21.11.15
#pragma region Func_Light

public:
	/// <summary>
	/// ���� ���ٽ� ������Ʈ
	/// 21.11.15
	/// </summary>
	/// <param name="sub_graphics"></param>
	virtual void update_stencil(SubGraphics* sub_graphics);

	/// <summary>
	/// ������ �޼���, ���۵� �������� Present�� �� ȣ���
	/// ����Ʈ ������Ʈ�� ���ۿ� ����Ʈ ����Ʈ�� ����
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