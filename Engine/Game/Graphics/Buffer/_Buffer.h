#pragma once

#include "../../_Include.h"

/*
* 버텍스 버퍼, 픽셀 버퍼, 인덱스 버퍼, 콘스탄트 버퍼 등을 정의하고 모아둠.
* 템플릿 메서드를 사용함. 설명은 Buffer에 있음.
*/
namespace BTYPE {}

/*
* 버텍스 버퍼 모음
* 
* static void T_get_input_layout(std::vector<D3D11_INPUT_ELEMENT_DESC>&)
* 스테틱 메서드로 이 버텍스 버퍼의 인풋 레이아웃을 반환하도록 작성해야 함.
* XMVECTOR T_get_position_vector() const;
* 인스턴스의 포지션을 반환하는 메서드
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
* 인덱스 버퍼 모음
* 
* static DXGI_FORMAT T_get_index_buffer_type();
* 인덱스 포멧 반환
* int T_get_index() const { return Index; }
* int형 인덱스로 반환
* 21.11.16
*/
namespace BTYPE
{
#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: unsigned long으로 index를 설정해도 int형으로 반환하면 의미가 없지 않나?")
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
* 콘스탄트 버퍼 모음
* 
* static UINT T_get_constant_buffer_slot();
* 입력할 콘스탄트 버퍼의 슬랏 설정. 13까지 가능한 것으로 알고 있음.
* static bool T_use_in_vs();
* vs에서 이 콘스탄트 버퍼를 사용할 것인가?
* static bool T_use_in_ps();
* ps에서 이 콘스탄트 버퍼를 사용할 것인가?
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