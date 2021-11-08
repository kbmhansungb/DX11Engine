#include "SubstancePipe.h"
#include "SubstancePipe_Renderer.h"
#include "SubstancePipe_Importer.h"
#include "../../../Component/Graphics/_SubGraphics.h"

BTYPE::BoneIDWeights::BoneIDWeights()
{}

void BTYPE::BoneIDWeights::T_get_input_layout(std::vector<D3D11_INPUT_ELEMENT_DESC>&Input_element_desc)
{
	for (UINT i = 0; i < DEFINE_MAX_BONE_LINK_SIZE; ++i)
	{
		Input_element_desc.push_back(
			D3D11_INPUT_ELEMENT_DESC{ "BONEIDS", i, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 2,
			D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 }
		);
	}
	for (UINT i = 0; i < DEFINE_MAX_BONE_LINK_SIZE; ++i)
	{
		Input_element_desc.push_back(
			D3D11_INPUT_ELEMENT_DESC{ "WEIGHTS", i, DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT, 2,
			D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 }
		);
	}
}

void BTYPE::BoneIDWeights::normalize()
{
	float sq = 0.f;
	for (int i = 0; i < DEFINE_MAX_BONE_LINK_SIZE; ++i)
	{
		sq += std::pow(Bone_Weight[i], 2.0);
	}
	if (sq == 0.0f) return;
	sq = std::sqrt(sq);
	for (int i = 0; i < DEFINE_MAX_BONE_LINK_SIZE; ++i)
	{
		Bone_Weight[i] /= sq;
	}
}

//////
// SubstanceMesh
//////

MESH::SubstanceMesh::~SubstanceMesh()
{

}

std::type_index MESH::SubstanceMesh::get_class_type() { return typeid(SubstanceMesh); }

void MESH::SubstanceMesh::load_resource(Engine* engine)
{
	this->buffer_vertex.initialize(engine->Device.Get(), vertex.data(), vertex.size());
	this->buffer_substance.initialize(engine->Device.Get(), substance.data(), vertex.size());
	this->buffer_bone_id_weights.initialize(engine->Device.Get(), bone_id_weights.data(), vertex.size());
	this->buffer_index.initialize(engine->Device.Get(), index.data(), index.size());
}

void MESH::SubstanceMesh::release_resource(Engine* engine)
{
	buffer_vertex.free_buffer();
	buffer_substance.free_buffer();
	buffer_bone_id_weights.free_buffer();
	buffer_index.free_buffer();
}

void MESH::SubstanceMesh::set_substance_mesh(SubGraphics* sub_graphics, UINT offset)
{
	// offsetÁ¦´ë·Î Àü´Þ ¾ÈÇØ¼­ ±úÁü.
	UINT strides[] = { buffer_vertex.Stride(), buffer_substance.Stride(), buffer_bone_id_weights.Stride() };
	ID3D11Buffer* buffer_address[] = { buffer_vertex.Get(), buffer_substance.Get(), buffer_bone_id_weights.Get() };
	UINT Offset[] = { 0, 0, 0 };
	sub_graphics->Device_context
		->IASetVertexBuffers(0, 3, buffer_address, strides, Offset);

	sub_graphics->Device_context
		->IASetIndexBuffer(buffer_index.Get(), buffer_index.get_index_buffer_type(), offset);
}

void MESH::SubstanceMesh::draw_substance_mesh(SubGraphics* sub_graphics)
{
	sub_graphics->Device_context->DrawIndexed(index.size(), 0, 0);
}

void MESH::SubstanceMesh::draw_detail_view()
{
	MESH::Mesh::draw_detail_view();

	if (ImGui::CollapsingHeader("Substance mesh"))
	{
		std::string name = "See the free view mesh##" + StringHelper::ptr_to_string(this);

		if (ImGui::TreeNode("Vertex info"))
		{
			for (size_t index = 0; index < vertex.size(); ++index)
			{
				if (ImGui::TreeNode(std::to_string(index).c_str()))
				{
					ImGui::Text("Positoin");
					ImGui::Text(std::to_string(vertex[index].Position.x).c_str()); ImGui::SameLine();
					ImGui::Text(std::to_string(vertex[index].Position.y).c_str()); ImGui::SameLine();
					ImGui::Text(std::to_string(vertex[index].Position.z).c_str());

					ImGui::Text("Texture coordinate");
					ImGui::Text(std::to_string(vertex[index].Texture_coordinate.x).c_str()); ImGui::SameLine();
					ImGui::Text(std::to_string(vertex[index].Texture_coordinate.y).c_str());

					ImGui::Text("Normal");
					ImGui::Text(std::to_string(vertex[index].Normal.x).c_str()); ImGui::SameLine();
					ImGui::Text(std::to_string(vertex[index].Normal.y).c_str()); ImGui::SameLine();
					ImGui::Text(std::to_string(vertex[index].Normal.z).c_str());

					ImGui::Text("Vertex color");
					ImGui::Text(std::to_string(substance[index].VertexColor.x).c_str()); ImGui::SameLine();
					ImGui::Text(std::to_string(substance[index].VertexColor.y).c_str()); ImGui::SameLine();
					ImGui::Text(std::to_string(substance[index].VertexColor.z).c_str()); ImGui::SameLine();
					ImGui::Text(std::to_string(substance[index].VertexColor.w).c_str());

					//ImGui::Text("Bone id and weight");
					//ImGui::Text(std::to_string(bone_id_weights[index].Bone_index.x).c_str()); ImGui::SameLine();
					//ImGui::Text(std::to_string(bone_id_weights[index].Bone_Weight.x).c_str());
					//ImGui::Text(std::to_string(bone_id_weights[index].Bone_index.y).c_str()); ImGui::SameLine();
					//ImGui::Text(std::to_string(bone_id_weights[index].Bone_Weight.y).c_str());
					//ImGui::Text(std::to_string(bone_id_weights[index].Bone_index.z).c_str()); ImGui::SameLine();
					//ImGui::Text(std::to_string(bone_id_weights[index].Bone_Weight.z).c_str());
					//ImGui::Text(std::to_string(bone_id_weights[index].Bone_index.w).c_str()); ImGui::SameLine();
					//ImGui::Text(std::to_string(bone_id_weights[index].Bone_Weight.w).c_str());

					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}

		if (ImGui::Button(name.c_str()))
		{
			auto shared = EDITOR::Shared::get_shared();

			std::string new_scene_name = "Free view - " + this->get_name().get_string();
			auto new_scene = shared->engine->root_scene.add_child(new_scene_name);

			//auto constructor = new_scene->root_game_object.add_child();
			//constructor.call_constructor(new Editor_SubstanceFreeView(this));
		}
	}
}

