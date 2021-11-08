#include "SubstancePipe_Renderer.h"
#include "SubstancePipe.h"
#include "SubstancePipe_Importer.h"
#include "../../../Component/Graphics/_SubGraphics.h"

//////
// BoneInstanceStruct

BoneInstanceStruct::BoneInstanceStruct() :
	parent_index(0),
	game_object(SafePtr<GameObject>())
{
}

BoneInstanceStruct::BoneInstanceStruct(UINT parent_index, SafePtr<GameObject> game_object) :
	parent_index(parent_index),
	game_object(game_object)
{
}

//////
// BoneRootComponent
//////

BoneRoot::BoneRoot()
{
}

void BoneRoot::create_bone()
{
	if (skeleton.is_vaild() == false) return;

	// ��Ʈ���� ��ȿ�� �� ��Ʈ���� ���ָ� �ڽ� ������ �˾Ƽ� ��������.
	if (bone_structs.size() > 0 &&
		bone_structs[0].game_object.is_vaild() == true)
	{
		owner->remove_child(bone_structs[0].game_object);
	}

	// ���� ���� ���� ����
	bone_structs.resize(skeleton->Bones.size());

	// 0�� ���� ���� ����� �����.
	bone_structs[0].game_object = owner->add_child(skeleton->Bones[0].Bone_name);
	bone_structs[0].parent_index = skeleton->Bones[0].Parent_bone;
	// child bone�� ������ ����
	for (size_t index = 1; index < skeleton->Bones.size(); ++index)
	{
		// �θ� �� �ε���
		bone_structs[index].parent_index = skeleton->Bones[index].Parent_bone;

		// ������Ʈ ����
		bone_structs[index].game_object
			= bone_structs[bone_structs[index].parent_index].game_object
			->add_child(skeleton->Bones[index].Bone_name);
		// Ʈ������ ����
		bone_structs[index].game_object->Adjust()
			.set_local_transform(skeleton->Bones[index].Local_transform);
	}

	// ���� ���� ���� ����
	bone_buffer.initialize(
		EngineContext::get_instance()->Device.Get(),
		bone_structs.size()
	);
}

void BoneRoot::set_bone_transform(SubGraphics* sub_graphics)
{
	for (size_t index = 0; index < bone_structs.size(); ++index)
	{
		bone_buffer.data[index] = bone_structs[index].game_object->world.data;
	}
	// ���� Ʈ������ ����.
	bone_buffer.apply_changes(EngineContext::get_instance()->Device_context.Get());
	bone_buffer.set_constant_buffer(sub_graphics->Device_context, 2);
}

void BoneRoot::draw_detail_view()
{
	Component::draw_detail_view();
	if (ImGui::CollapsingHeader("BoneRoot"))
	{
		ImGui::base_field(skeleton, "skeleton");

		ImGui::NewLine();
		std::string create_bone_button_name = "Create bone##" + StringHelper::ptr_to_string(this);
		if (ImGui::RightAngleButton(create_bone_button_name.c_str()))
		{
			create_bone();
		}

#pragma message(__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: ImGui::uint_field�� ����� �߰��ؾ���.")
		ImGui::vector_field(this->bone_structs,
			[=](size_t index, BoneInstanceStruct& bis)
			{
				//
			});
	}
}

//////
// BoneChild

void BoneChild::create_bone()
{
	if (skeleton.is_vaild() == false) return;

	// �� ��Ʈ��Ʈ ����
	bone_structs.resize(skeleton->Bones.size());

	// 0�� ���� ���� ����� �����.
	bone_structs[0].game_object = owner->find_child_by_name(skeleton->Bones[0].Bone_name);
	bone_structs[0].parent_index = skeleton->Bones[0].Parent_bone;
	// �� ����.
	for (size_t index = 1; index < skeleton->Bones.size(); ++index)
	{
		// ������Ʈ Ž�� ���� ��� �Ҵ�. ������ ����.
		SafePtr<GameObject> now_bone
			= owner->find_child_by_name(skeleton->Bones[index].Bone_name);
		if (now_bone.is_vaild() == false)
		{
			// Ž���ؼ� ���� ��� ���� ����� �����.
			SafePtr<GameObject> parent_bone
				= owner->find_child_by_name(
					skeleton->Bones[skeleton->Bones[index].Parent_bone].Bone_name
				);
			now_bone = parent_bone->add_child(skeleton->Bones[index].Bone_name);
		}
#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: �θ� ���� ���°��� ��� ����.")
		bone_structs[index].game_object = now_bone;
		//�θ� �� �ε���
		bone_structs[index].parent_index = skeleton->Bones[index].Parent_bone;
	}

	// ���� ����
	bone_buffer.initialize(
		EngineContext::get_instance()->Device.Get(),
		bone_structs.size()
	);
}

void BoneChild::draw_detail_view()
{
	BoneRoot::draw_detail_view();
	if (ImGui::CollapsingHeader("BoneChild"))
	{
		ImGui::base_field(bone_root);
	}
}

//////
// SusbstancePipe
//////

bool SubstancePipe::is_draw_able()
{
	if (bone.is_vaild() == false) return false;
	if (shader.is_vaild() == false) return false;
	if (mesh.is_vaild() == false) return false;
	return true;
}

static SHADER::VertexShader vs;
static void smart_load(SubGraphics* sub_graphics)
{
	if (vs.GetShader() == nullptr)
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> input_layout;
		BTYPE::Vertex_default::T_get_input_layout(input_layout);
		BTYPE::Substance::T_get_input_layout(input_layout);
		BTYPE::BoneIDWeights::T_get_input_layout(input_layout);

		vs.initialize(
			sub_graphics->Device,
			L"vs_Substance",
			input_layout
		);
	}
}
void SubstancePipe::draw(SubGraphics* sub_graphics)
{
	if (is_draw_able() == false) return;

	shader->set_mesh_shader(sub_graphics);
	draw_mesh_only(sub_graphics);
}

void SubstancePipe::draw_mesh_only(SubGraphics* sub_graphics)
{
	if (is_draw_able() == false) return;

	smart_load(sub_graphics);
	sub_graphics->Device_context->VSSetShader(
		vs.GetShader(),
		0,
		0
	);
	sub_graphics->Device_context->IASetInputLayout(
		vs.GetInputLayout()
	);
	bone->set_bone_transform(sub_graphics);
	mesh->set_substance_mesh(sub_graphics);
	mesh->draw_substance_mesh(sub_graphics);
}

void SubstancePipe::draw_substance_pipe_detail()
{
	ImGui::base_field(bone, "bone");
	ImGui::base_field(shader, "shader");
	ImGui::base_field(mesh, "mesh");
}

//////
// SubstanceRenderer
//////

SubstanceRenderer::SubstanceRenderer()
{
}

SubstanceRenderer::SubstanceRenderer(SafePtr<MESH::SubstanceMesh> substance_mesh)
{
	this->Substance_pipe.mesh = substance_mesh;
	this->Substance_pipe.shader = substance_mesh->default_shader;
}

std::type_index SubstanceRenderer::get_class_type()
{
	return typeid(SubstanceRenderer);
}

//#include "../SpritePipe/SpritePipe.h"
void SubstanceRenderer::draw(SubGraphics* sub_graphics)
{
	//// test
	{
	//	// Graphics�� �۵��ϴ��� Ȯ���ϱ� ���� �۾�.
	//	MESH::SpriteMesh sprite_mesh;
	//	sprite_mesh.sprite_vertex.emplace_back(-1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	//	sprite_mesh.sprite_vertex.emplace_back(1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	//	sprite_mesh.sprite_vertex.emplace_back(1.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	//	sprite_mesh.sprite_vertex.emplace_back(-1.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	//	sprite_mesh.sprite_index.emplace_back(0);
	//	sprite_mesh.sprite_index.emplace_back(1);
	//	sprite_mesh.sprite_index.emplace_back(2);
	//	SHADER::SpriteShader sprite_shader(L"Sprite_vs_shader", L"Sprite_ps_shader");
	//	sprite_shader.rasterizer_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	//	// load
	//	sprite_shader.load_resource(this->owner->this_scene->this_engine.get());
	//	sprite_mesh.load_resource(this->owner->this_scene->this_engine.get());
	//
	//	SpritePipe pipe;
	//	pipe.sprite_mesh = sprite_mesh.get_safe_ptr().cast<MESH::SpriteMesh>();
	//	pipe.sprite_shader = sprite_shader.get_safe_ptr().cast<SHADER::SpriteShader>();
	//	pipe.draw(sub_graphics);
	}

	this->Substance_pipe.draw(sub_graphics);
}

void SubstanceRenderer::draw_mesh_only(SubGraphics* sub_graphics)
{
	Substance_pipe.draw_mesh_only(sub_graphics);
}

void SubstanceRenderer::draw_detail_view()
{
	ViaRenderer::draw_detail_view();

	if (ImGui::CollapsingHeader("Substance renderer"))
	{
		this->Substance_pipe.draw_substance_pipe_detail();
	}
}