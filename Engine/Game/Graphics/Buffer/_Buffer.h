#pragma once

#include "../../_Include.h"

// Vertex buffer
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
// Index buffer
namespace BTYPE
{
	struct IB_Default
	{
		unsigned long Index;
		IB_Default() : Index() {}
		IB_Default(unsigned long Index) : Index(Index) {}

		static DXGI_FORMAT T_get_index_buffer_type()
		{
			return DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
		}
		int T_get_index() const { return Index; }
	};
}

// Constant buffer
namespace BTYPE
{
	struct CB_World_only
	{
	private:
		XMMATRIX _world;
	public:
		CB_World_only() : _world(DirectX::XMMatrixIdentity()) {}
		CB_World_only(DirectX::XMMATRIX& world) : _world(world) {}

		void put_world(const XMMATRIX& new_world) { _world = new_world; }
		const XMMATRIX& get_world() { return _world; }
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

		void put_viewprojection(XMMATRIX new_world) { _viewprojection = new_world, _viewprojection_inverse = XMMatrixInverse(nullptr, new_world); }
		XMMATRIX get_viewprojection() { return _viewprojection; }
		XMMATRIX get_viewprojection_inverse() { return _viewprojection_inverse; }
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