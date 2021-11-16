#pragma once

#include "../../_Include.h"

/*
* ���ؽ� ����, �ȼ� ����, �ε��� ����, �ܽ�źƮ ���� ���� �����ϰ� ��Ƶ�.
* ���ø� �޼��带 �����. ������ Buffer�� ����.
*/
namespace BTYPE {}

/*
* ���ؽ� ���� ����
* 
* static void T_get_input_layout(std::vector<D3D11_INPUT_ELEMENT_DESC>&)
* ����ƽ �޼���� �� ���ؽ� ������ ��ǲ ���̾ƿ��� ��ȯ�ϵ��� �ۼ��ؾ� ��.
* XMVECTOR T_get_position_vector() const;
* �ν��Ͻ��� �������� ��ȯ�ϴ� �޼���
* 
* 21.11.16
*/
namespace BTYPE
{
	struct Vertex_default
	{
		XMFLOAT3 Position;
		XMFLOAT2 Texture_coordinate;
		XMFLOAT3 Normal;

		Vertex_default();
		Vertex_default(float pos_x, float pos_y, float pos_z, float u, float v, float normal_x, float normal_y, float normal_z);

		static void T_get_input_layout(std::vector<D3D11_INPUT_ELEMENT_DESC>& Input_element_desc);
		XMVECTOR T_get_position_vector() const;
	};

	struct Substance
	{
		DirectX::XMFLOAT4 VertexColor;

		Substance();
		Substance(float color_x, float color_y, float color_z, float color_w);

		static void T_get_input_layout(std::vector<D3D11_INPUT_ELEMENT_DESC>& Input_element_desc);
	};
}

/*
* �ε��� ���� ����
* 
* static DXGI_FORMAT T_get_index_buffer_type();
* �ε��� ���� ��ȯ
* int T_get_index() const { return Index; }
* int�� �ε����� ��ȯ
* 21.11.16
*/
namespace BTYPE
{
#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: unsigned long���� index�� �����ص� int������ ��ȯ�ϸ� �ǹ̰� ���� �ʳ�?")
	struct IB_Default
	{
		unsigned long Index;
		IB_Default() : Index() {}
		IB_Default(unsigned long Index) : Index(Index) {}

		static DXGI_FORMAT T_get_index_buffer_type();
		int T_get_index() const;
	};
}

/*
* �ܽ�źƮ ���� ����
* 
* static UINT T_get_constant_buffer_slot();
* �Է��� �ܽ�źƮ ������ ���� ����. 13���� ������ ������ �˰� ����.
* static bool T_use_in_vs();
* vs���� �� �ܽ�źƮ ���۸� ����� ���ΰ�?
* static bool T_use_in_ps();
* ps���� �� �ܽ�źƮ ���۸� ����� ���ΰ�?
*/
namespace BTYPE
{
	struct CB_World_only
	{
	private:
		XMMATRIX _world;
	public:
		CB_World_only() : _world(DirectX::XMMatrixIdentity()) {}
		CB_World_only(DirectX::XMMATRIX& world) : _world(world) {}

		void put_world(const XMMATRIX& new_world);
		const XMMATRIX& get_world();
		__declspec(property(get = get_world, put = put_world)) XMMATRIX world;

		static UINT T_get_constant_buffer_slot() { return 0; }
		static bool T_use_in_vs() { return true; }
		static bool T_use_in_ps() { return true; }
	};

	struct CB_World
	{
	private:
		XMMATRIX _world;
		XMMATRIX _world_inverse;
	public:
		CB_World() : _world(DirectX::XMMatrixIdentity()), _world_inverse(DirectX::XMMatrixIdentity()) {}
		CB_World(DirectX::XMMATRIX& world) : _world(world), _world_inverse(DirectX::XMMatrixInverse(nullptr, world)) {}

		void put_world(const XMMATRIX& new_world) { _world = new_world, _world_inverse = XMMatrixInverse(nullptr, new_world); }
		const XMMATRIX& get_world() { return _world; }
		XMMATRIX get_world_inverse() { return _world_inverse; }
		__declspec(property(get = get_world, put = put_world)) XMMATRIX world;
		__declspec(property(get = get_world_inverse)) XMMATRIX world_inverse;

		static UINT T_get_constant_buffer_slot() { return 0; }
		static bool T_use_in_vs() { return true; }
		static bool T_use_in_ps() { return true; }
	};

	struct CB_ViewProjection
	{
	private:
		DirectX::XMMATRIX _viewprojection;
		DirectX::XMMATRIX _viewprojection_inverse;
	public:
		CB_ViewProjection() : _viewprojection(DirectX::XMMatrixIdentity()), _viewprojection_inverse(DirectX::XMMatrixIdentity()) {}
		CB_ViewProjection(DirectX::XMMATRIX& viewprojection) : _viewprojection(viewprojection), _viewprojection_inverse(DirectX::XMMatrixInverse(nullptr, viewprojection)) {}

		void put_viewprojection(XMMATRIX new_world);
		XMMATRIX get_viewprojection();
		XMMATRIX get_viewprojection_inverse();
		__declspec(property(get = get_viewprojection, put = put_viewprojection)) XMMATRIX Viewprojection;
		__declspec(property(get = get_viewprojection_inverse)) XMMATRIX Viewprojection_inverse;

		static UINT T_get_constant_buffer_slot() { return 1; }
		static bool T_use_in_vs() { return true; }
		static bool T_use_in_ps() { return true; }
	};

	struct CB_DirectLight
	{
		DirectX::XMFLOAT3 Ambient_color;
		float Ambient_strength;
		DirectX::XMFLOAT3 Light_color;
		float Light_strength;
		DirectX::XMFLOAT3 Light_direction;

		CB_DirectLight() : Ambient_color(), Ambient_strength(), Light_direction(), Light_color(), Light_strength() {}

		static UINT T_get_constant_buffer_slot() { return 2; }
		static bool T_use_in_vs() { return false; }
		static bool T_use_in_ps() { return true; }
	};
}