#include "_Buffer.h"

using namespace std;

BTYPE::Vertex_default::Vertex_default() {}

BTYPE::Vertex_default::Vertex_default(float pos_x, float pos_y, float pos_z, float u, float v, float nor_x, float nor_y, float nor_z) :
	Position(pos_x, pos_y, pos_z),
	Texture_coordinate(u, v),
	Normal(nor_x, nor_y, nor_z)
{
}

void BTYPE::Vertex_default::T_get_input_layout(vector<D3D11_INPUT_ELEMENT_DESC>& Input_element_desc)
{
	Input_element_desc.push_back(
		D3D11_INPUT_ELEMENT_DESC{ "POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0,
		D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 }
	);
	Input_element_desc.push_back(
		D3D11_INPUT_ELEMENT_DESC{ "TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0,
		D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 }
	);
	Input_element_desc.push_back(
		D3D11_INPUT_ELEMENT_DESC{ "NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0,
		D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 }
	);
}

XMVECTOR BTYPE::Vertex_default::T_get_position_vector() const
{
	return XMVectorSet(Position.x, Position.y, Position.z, 1.0f);
}

BTYPE::Substance::Substance() : VertexColor() {}

BTYPE::Substance::Substance(float color_x, float color_y, float color_z, float color_w) :
	VertexColor(color_x, color_y, color_z, color_w)
{}

void BTYPE::Substance::T_get_input_layout(vector<D3D11_INPUT_ELEMENT_DESC>&Input_element_desc)
{
	Input_element_desc.push_back(
		D3D11_INPUT_ELEMENT_DESC{ "COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 1,
		D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 }
	);
}

DXGI_FORMAT BTYPE::IB_Default::T_get_index_buffer_type()
{
	return DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
}

int BTYPE::IB_Default::T_get_index() const { return Index; }

void BTYPE::CB_World_only::put_world(const XMMATRIX& new_world) { _world = new_world; }

const XMMATRIX& BTYPE::CB_World_only::get_world() { return _world; }

void BTYPE::CB_ViewProjection::put_viewprojection(XMMATRIX new_world) { _viewprojection = new_world, _viewprojection_inverse = XMMatrixInverse(nullptr, new_world); }

XMMATRIX BTYPE::CB_ViewProjection::get_viewprojection() { return _viewprojection; }

XMMATRIX BTYPE::CB_ViewProjection::get_viewprojection_inverse() { return _viewprojection_inverse; }
