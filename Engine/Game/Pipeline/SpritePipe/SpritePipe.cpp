#include "SpritePipe.h"
#include "../../Engine.h"
#include "../../../Component/Graphics/_SubGraphics.h"

using namespace std;

// Sprite shader

// Sprite mesh

MESH::SpriteMesh::~SpriteMesh()
{
}

void MESH::SpriteMesh::load_resource(Engine* engine)
{
	this->buffer_sprite_vertex.initialize(engine->Device.Get(), sprite_vertex.data(), sprite_vertex.size());
	this->buffer_sprite_index.initialize(engine->Device.Get(), sprite_index.data(), sprite_index.size());
}

void MESH::SpriteMesh::release_resource(Engine* engine)
{
	buffer_sprite_vertex.free_buffer();
	buffer_sprite_index.free_buffer();
}

void MESH::SpriteMesh::set_sprite_mesh(SubGraphics* sub_graphics, UINT offset)
{
	sub_graphics->Device_context
		->IASetVertexBuffers(0, 1, buffer_sprite_vertex.GetAddressOf(), buffer_sprite_vertex.StridePtr(), &offset);
	sub_graphics->Device_context
		->IASetIndexBuffer(buffer_sprite_index.Get(), buffer_sprite_index.get_index_buffer_type(), offset);
}

void MESH::SpriteMesh::draw_sprite_mesh(SubGraphics* sub_graphics)
{
	sub_graphics->Device_context->DrawIndexed(this->sprite_index.size(), 0, 0);
}

void MESH::SpriteMesh::draw_detail_view()
{

}

// Pipe

static SHADER::VertexShader vs;
static void smart_load(SubGraphics* sub_graphics)
{
	if (vs.GetShader() == nullptr)
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> input_layout;
		BTYPE::Vertex_default::T_get_input_layout(input_layout);

		vs.initialize(
			sub_graphics->Device,
			L"vs_Sprite",
			input_layout
		);
	}
}
bool SpritePipe::is_draw_able()
{
	if (sprite_shader.is_vaild() == false) return false;
	if (sprite_mesh.is_vaild() == false) return false;
	return true;
}
void SpritePipe::draw(SubGraphics* sub_graphics, ConstantBuffer<BTYPE::CB_World>& world)
{
	if (is_draw_able()) return;

	sprite_mesh->set_sprite_mesh(sub_graphics);
	draw_mesh_only(sub_graphics, world);
}

void SpritePipe::draw_mesh_only(SubGraphics* sub_graphics, ConstantBuffer<BTYPE::CB_World>& world)
{
	if (is_draw_able()) return;

	set_sprite_vs(sub_graphics);
	world.set_constant_buffer(sub_graphics->Device_context);
	sprite_mesh->set_sprite_mesh(sub_graphics);
	sprite_mesh->draw_sprite_mesh(sub_graphics);
}

void SpritePipe::set_sprite_vs(SubGraphics* sub_graphics)
{
	smart_load(sub_graphics);
	sub_graphics->Device_context->VSSetShader(
		vs.GetShader(),
		0,
		0
	);
	sub_graphics->Device_context->IASetInputLayout(
		vs.GetInputLayout()
	);
}

SafePtr<MESH::SpriteMesh> MESH::default_sprite_mesh(Engine* engine)
{
	Name default_sprite_mesh_name =
		Name(&engine->name_container, "Default_sprite_mesh");

	SafePtr<MESH::SpriteMesh> default_sprite_mesh
		= engine->resources.get_resource(default_sprite_mesh_name).cast<MESH::SpriteMesh>();

	if (default_sprite_mesh.is_vaild() == false)
	{
		std::unique_ptr<MESH::SpriteMesh> new_sprit_mesh
			= std::make_unique<MESH::SpriteMesh>();

		new_sprit_mesh->sprite_vertex.resize(4);
		new_sprit_mesh->sprite_vertex[0].Position = XMFLOAT3(-1.f, 1.f, 0.f);
		new_sprit_mesh->sprite_vertex[1].Position = XMFLOAT3(1.f, 1.f, 0.f);
		new_sprit_mesh->sprite_vertex[2].Position = XMFLOAT3(1.f, -1.f, 0.f);
		new_sprit_mesh->sprite_vertex[3].Position = XMFLOAT3(-1.f, -1.f, 0.f);
		new_sprit_mesh->sprite_vertex[0].Texture_coordinate = XMFLOAT2(0.f, 0.f);
		new_sprit_mesh->sprite_vertex[1].Texture_coordinate = XMFLOAT2(1.f, 0.f);
		new_sprit_mesh->sprite_vertex[2].Texture_coordinate = XMFLOAT2(1.f, 1.f);
		new_sprit_mesh->sprite_vertex[3].Texture_coordinate = XMFLOAT2(0.f, 1.f);
		new_sprit_mesh->sprite_index.resize(6);
		new_sprit_mesh->sprite_index[0].Index = 0;
		new_sprit_mesh->sprite_index[1].Index = 1;
		new_sprit_mesh->sprite_index[2].Index = 3;
		new_sprit_mesh->sprite_index[3].Index = 1;
		new_sprit_mesh->sprite_index[4].Index = 2;
		new_sprit_mesh->sprite_index[5].Index = 3;
		new_sprit_mesh->load_resource(engine);

		default_sprite_mesh = engine->resources
			.add_resource(
				default_sprite_mesh_name,
				new_sprit_mesh);
	}

	return default_sprite_mesh;
}

SHADER::MeshShader::MeshShader()
{
	raw_ms.rasterizer_desc = CD3D11_RASTERIZER_DESC(D3D11_DEFAULT);
	raw_ms.rasterizer_desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
	raw_ms.rasterizer_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;

	raw_ms.depth_stencil_desc = CD3D11_DEPTH_STENCIL_DESC(D3D11_DEFAULT);
	raw_ms.depth_stencil_desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

	addition_ms.topology = D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	D3D11_RENDER_TARGET_BLEND_DESC render_target_blend_desc_1{};
	render_target_blend_desc_1.BlendEnable = true;
	render_target_blend_desc_1.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	render_target_blend_desc_1.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
	render_target_blend_desc_1.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	render_target_blend_desc_1.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
	render_target_blend_desc_1.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
	render_target_blend_desc_1.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	render_target_blend_desc_1.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;
	// ( Src * ( 1 - a ) ) + ( Dest * a )
	// ?????

	D3D11_RENDER_TARGET_BLEND_DESC render_target_blend_desc_2{};
	render_target_blend_desc_2.BlendEnable = true;
	render_target_blend_desc_2.SrcBlend = D3D11_BLEND::D3D11_BLEND_ONE;
	render_target_blend_desc_2.DestBlend = D3D11_BLEND::D3D11_BLEND_ONE;
	render_target_blend_desc_2.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	render_target_blend_desc_2.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
	render_target_blend_desc_2.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
	render_target_blend_desc_2.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	render_target_blend_desc_2.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

	raw_ms.blend_describe.AlphaToCoverageEnable = true;
	raw_ms.blend_describe.IndependentBlendEnable = true;
	raw_ms.blend_describe.RenderTarget[0] = render_target_blend_desc_1;
	raw_ms.blend_describe.RenderTarget[1] = render_target_blend_desc_1;
	raw_ms.blend_describe.RenderTarget[2] = render_target_blend_desc_1;
	raw_ms.blend_describe.RenderTarget[3] = render_target_blend_desc_2;
	raw_ms.blend_describe.RenderTarget[4] = render_target_blend_desc_1;
	raw_ms.blend_describe.RenderTarget[5] = render_target_blend_desc_1;
	raw_ms.blend_describe.RenderTarget[6] = render_target_blend_desc_1;
	raw_ms.blend_describe.RenderTarget[7] = render_target_blend_desc_1;

	raw_ms.pixel_shader_path = L"ps_Rendering";
}

void SHADER::MeshShader::draw_detail_view()
{
	Shader::draw_detail_view();

	if (ImGui::CollapsingHeader("Mesh shader"))
	{
		ImGui::wstring_field(raw_ms.pixel_shader_path, "pixel shader path", 100);

		if (ImGui::TreeNode("Texture state"))
		{
			if (ImGui::TreeNode("VS texture"))
			{
				ImGui::map_field_default(
					addition_ms.vs_textures,
					static_cast<UINT>(0),
					SafePtr<TEXTURE::Texture>(),
					[=](const UINT& slot, UINT& new_slot) -> bool
					{
						int field = slot;
						int old_slot = field;
						std::string index_name
							= "Slot##"
							+ StringHelper::ptr_to_string(const_cast<UINT*>(&slot));
						ImGui::InputInt(index_name.c_str(), &field);

						if (field != old_slot)
						{
							new_slot = field;
							return true;
						}
						return false;
					},
					[=](const UINT& slot, SafePtr<TEXTURE::Texture>& texture)
					{
						ImGui::base_field(texture);
					}
					);

				ImGui::TreePop();
			}
			if (ImGui::TreeNode("PS texture"))
			{
				ImGui::map_field_default(
					addition_ms.ps_textures,
					static_cast<UINT>(0),
					SafePtr<TEXTURE::Texture>(),
					[=](const UINT& slot, UINT& new_slot) -> bool
					{
						int field = slot;
						int old_slot = field;
						std::string index_name
							= "Slot##"
							+ StringHelper::ptr_to_string(const_cast<UINT*>(&slot));
						ImGui::InputInt(index_name.c_str(), &field);

						if (field != old_slot)
						{
							new_slot = field;
							return true;
						}
						return false;
					},
					[=](const UINT& slot, SafePtr<TEXTURE::Texture>& texture)
					{
						ImGui::base_field(texture);
					}
					);

				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
	}
}

void SHADER::MeshShader::load_resource(Engine* engine)
{
	SHADER::MeshShader::release_resource(engine);
	SHADER::Shader::load_resource(engine);

	HRESULT hr;

	hr = engine->Device->CreateRasterizerState(
		&raw_ms.rasterizer_desc,
		product_ms.rasterizer_state.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create rasterizer state.");

	hr = engine->Device->CreateBlendState(
		&raw_ms.blend_describe,
		product_ms.blend_state.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to blend state.");

	hr = engine->Device->CreateDepthStencilState(
		&raw_ms.depth_stencil_desc,
		product_ms.depth_stencil_state.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil state.");

	product_ms.pixel_shader.initialize(engine->Device.Get(), raw_ms.pixel_shader_path);
}

void SHADER::MeshShader::release_resource(Engine* engine)
{
	SHADER::Shader::release_resource(engine);

	product_ms.rasterizer_state.Reset();
	product_ms.blend_state.Reset();
	product_ms.depth_stencil_state.Reset();
	product_ms.pixel_shader.free_shader();
}

void SHADER::MeshShader::set_mesh_shader(SubGraphics* sub_graphics)
{
	sub_graphics->Device_context->RSSetState(product_ms.rasterizer_state.Get());
	sub_graphics->Device_context->OMSetBlendState(product_ms.blend_state.Get(), nullptr, 0xffffffff);
	sub_graphics->Device_context->OMSetDepthStencilState(product_ms.depth_stencil_state.Get(), 0);

	// 두번 할당하지 않기 위해서는 draw_mesh_only에서 호출해야 한다.
	//this->set_texture(sub_graphics);

	sub_graphics->Device_context->IASetPrimitiveTopology(addition_ms.topology);
	sub_graphics->Device_context->PSSetShader(product_ms.pixel_shader.GetShader(), 0, 0);
}

void SHADER::MeshShader::set_texture(SubGraphics* sub_graphics)
{
	for (auto it = addition_ms.vs_textures.begin(); it != addition_ms.vs_textures.end(); ++it)
	{
		if (it->first < 0) continue;
		if (it->second.is_vaild() == false) continue;
		sub_graphics->Device_context
			->VSSetShaderResources(it->first, 1, it->second.get()->GetTextureResourceViewAddress());
		sub_graphics->Device_context
			->VSSetSamplers(it->first, 1, it->second.get()->sampler_state.GetAddressOf());
	}
	for (auto it = addition_ms.ps_textures.begin(); it != addition_ms.ps_textures.end(); ++it)
	{
		if (it->first < 0) continue;
		if (it->second.is_vaild() == false) continue;
		sub_graphics->Device_context
			->PSSetShaderResources(it->first, 1, it->second.get()->GetTextureResourceViewAddress());
		sub_graphics->Device_context
			->PSSetSamplers(it->first, 1, it->second.get()->sampler_state.GetAddressOf());
	}
}
