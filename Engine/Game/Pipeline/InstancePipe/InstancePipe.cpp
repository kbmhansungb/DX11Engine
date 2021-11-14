#include "InstancePipe.h"
#include "../../../Component/Graphics/_Exclude.h"

//void SHADER::RESOURCECONSTRUCT::ResourceConstruct_InstanceShader::create_resource(std::unique_ptr<MeshShader>& resource)
//{
//	resource = std::make_unique<MeshShader>();
//}

void InstancePipe::initialize(Engine* engine)
{
	worlds.initialize(engine->Device.Get());
	worlds.apply_changes(engine->Device_context.Get());
}

static SHADER::VertexShader vs;
static void smart_load(SubGraphics* sub_graphics)
{
	if (vs.GetShader() == nullptr)
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> input_layout;
		BTYPE::Vertex_default::T_get_input_layout(input_layout);

		vs.initialize(
			sub_graphics->Device,
			L"vs_Instance",
			input_layout
		);
	}
}
void InstancePipe::draw(SubGraphics* sub_graphics, ConstantBuffer<BTYPE::CB_World>& world)
{
#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: �ѹ��� �׸� �� �ִ� �ν��Ͻ� ũ�� ������ �ִµ� �ϴ�..?")
#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: �װ��� ����̶�� �ڵ带 �ٲ�� �ϳ�?")
#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: ConstantBuffer�� �ν��Ͻ� �������� �ܽ�źƮ ���۸� ��������� ���ľ� �ϰ�.")
#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: �ѹ��� �׸��� �ν��Ͻ��� ������ ��� ���������� �����ϰ� ó�� ����")
#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: Constant buffer�� �ִ����� 2048����Ʈ ��µ�??..")
	if (is_draw_able() == false) return;

	shader->set_mesh_shader(sub_graphics);
	draw_mesh_only(sub_graphics, world);
}

void InstancePipe::draw_mesh_only(SubGraphics* sub_graphics, ConstantBuffer<BTYPE::CB_World>& world)
{
	if (is_draw_able() == false) return;

	shader->set_texture(sub_graphics);
	smart_load(sub_graphics);
	sub_graphics->Device_context->VSSetShader(
		vs.GetShader(),
		0,
		0
	);
	sub_graphics->Device_context->IASetInputLayout(
		vs.GetInputLayout()
	);

	world.set_constant_buffer(sub_graphics->Device_context);
	worlds.apply_changes(sub_graphics->Device_context);
	worlds.set_constant_buffer(sub_graphics->Device_context, 2);

	mesh->set_mesh_mesh(sub_graphics);
	UINT index_size = mesh->product_mm.index.size();
	UINT world_size = this->worlds.data.size();
	sub_graphics->Device_context->DrawIndexedInstanced(
		index_size,
		world_size,
		0,
		0,
		0);
}

void InstancePipe::update()
{
	ID3D11Device* device;
	if (worlds.Get() == nullptr) return;
	worlds.Get()->GetDevice(&device);
	worlds.initialize(device);
}

void InstancePipe::draw_instance_detail_view()
{
	if (ImGui::TreeNode("Instance worlds"))
	{
		if (ImGui::RightAngleButton(("Add##" + StringHelper::ptr_to_string(&worlds.data)).c_str()))
		{
			worlds.data.emplace_back();
		}
		int Delete_index = -1;
		ImGui::vector_field(worlds.data,
			[&](size_t index, BTYPE::CB_World_only& cb_world_only)
			{
				if (ImGui::TreeNode((std::to_string(index) + "##" + StringHelper::ptr_to_string(&cb_world_only)).c_str()))
				{
					if (ImGui::RightAngleButton(("Delete##" + StringHelper::ptr_to_string(&cb_world_only)).c_str()))
					{
						Delete_index = index;
					}
					XMMATRIX world = cb_world_only.world;
					ImGui::XMMATRIX_field(world, "world");
					cb_world_only.world = world;

					ImGui::TreePop();
				}
			});
		if (Delete_index != -1)
		{
			worlds.data.erase(worlds.data.begin() + Delete_index);
		}
		ImGui::TreePop();
	}
	update();
}

std::type_index InstanceMeshRenderer::get_class_type()
{
	return typeid(InstanceMeshRenderer);
}

void InstanceMeshRenderer::draw(SubGraphics* sub_graphics)
{
	this->instance_pipe.draw(sub_graphics, owner->world);

	//sub_graphics->Device_context->DrawInstanced(100, 100, 0, 0);
}

void InstanceMeshRenderer::draw_mesh_only(SubGraphics* sub_graphics)
{
	instance_pipe.draw_mesh_only(sub_graphics, owner->world);
}

void InstanceMeshRenderer::draw_detail_view()
{
	ViaRenderer::draw_detail_view();

	if (ImGui::CollapsingHeader("InstanceMeshRenderer"))
	{
		ImGui::base_field(this->instance_pipe.mesh, "Mesh");
		ImGui::base_field(this->instance_pipe.shader, "World");

		instance_pipe.draw_instance_detail_view();
	}
}

void InstanceMeshRenderer::awake()
{
	instance_pipe.initialize(this->owner->this_scene->this_engine.get());
}
