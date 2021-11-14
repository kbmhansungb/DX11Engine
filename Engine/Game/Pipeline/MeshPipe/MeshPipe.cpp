#include "MeshPipe.h"

#include "../../Engine.h"
#include "../../../Component/Graphics/_SubGraphics.h"
#include "../../../Template/Editor_FreeView/Editor_FreeView.h"
#include "../../../Template/Editor_ImageView/Editor_ImageView.h"

using namespace std;

MESH::MeshMesh::~MeshMesh()
{
}

std::type_index MESH::MeshMesh::get_class_type()
{
	return typeid(MESH::MeshMesh);
}

void MESH::MeshMesh::load_resource(Engine* engine)
{
	this->product_mm.buffer_vertex.initialize(engine->Device.Get(), this->product_mm.vertex.data(), this->product_mm.vertex.size());
	this->product_mm.buffer_index.initialize(engine->Device.Get(), this->product_mm.index.data(), this->product_mm.index.size());
}

void MESH::MeshMesh::release_resource(Engine* engine)
{
	this->product_mm.buffer_vertex.free_buffer();
	this->product_mm.buffer_index.free_buffer();
}

void MESH::MeshMesh::set_mesh_mesh(SubGraphics* sub_graphics, UINT offset)
{
	// offset제대로 전달 안해서 깨짐.
	UINT strides[] = { this->product_mm.buffer_vertex.Stride() };
	ID3D11Buffer* buffer_address[] = { this->product_mm.buffer_vertex.Get() };
	UINT Offset[] = { 0 };
	sub_graphics->Device_context
		->IASetVertexBuffers(0, 1, buffer_address, strides, Offset);

	sub_graphics->Device_context
		->IASetIndexBuffer(this->product_mm.buffer_index.Get(), this->product_mm.buffer_index.get_index_buffer_type(), offset);
}

void MESH::MeshMesh::draw_mesh_mesh(SubGraphics* sub_graphics)
{
	sub_graphics->Device_context->DrawIndexed(this->product_mm.index.size(), 0, 0);
}

void MESH::MeshMesh::draw_detail_view()
{
	MESH::Mesh::draw_detail_view();

	if (ImGui::CollapsingHeader("Mesh mesh"))
	{
		ImGui::Text_field(raw_mm.source_path, "##Source path", 100);
		// Path select
		static unique_ptr<ImGui::FileBrowser> FB;
		if (ImGui::RightAngleButton("Select path")
			&& FB.get() == nullptr)
		{
			FB = make_unique<ImGui::FileBrowser>();

			FB->Open();
			FB->SetTitle("Importer");
			FB->SetTypeFilters({ ".fbx" });
		}
		if (FB.get() != nullptr)
		{
			FB->Display();

			if (FB->HasSelected())
			{
				raw_mm.source_path = FB->GetSelected().string();
				FB->ClearSelected();
				FB->Close();
			}
		}
		ImGui::NewLine();
		if (ImGui::RightAngleButton(("Free view##" + StringHelper::ptr_to_string(this)).c_str()))
		{
			EDITOR::free_view_mesh_mesh_renderer(this);
		}
		ImGui::NewLine();

		if (ImGui::TreeNode("Addition"))
		{
			ImGui::base_field(this->addition_mm.default_shader, "Default shader");
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Vertex info"))
		{
			for (size_t index = 0; index < this->product_mm.vertex.size(); ++index)
			{
				if (ImGui::TreeNode((to_string(index) + "##vertex").c_str()))
				{
					ImGui::XMFLOAT3_field(this->product_mm.vertex[index].Position, "Position");
					ImGui::XMFLOAT2_field(this->product_mm.vertex[index].Texture_coordinate, "Texture coordinate");
					ImGui::XMFLOAT3_field(this->product_mm.vertex[index].Normal, "Normal");

					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Index info"))
		{
			for (size_t index = 0; index < this->product_mm.index.size() / 3; ++index)
			{
				if (ImGui::TreeNode((to_string(index) + "##face").c_str()))
				{
					static int int_3[3]{};
					int_3[0] = this->product_mm.index[index * 3 + 0].Index;
					int_3[1] = this->product_mm.index[index * 3 + 1].Index;
					int_3[2] = this->product_mm.index[index * 3 + 2].Index;
					ImGui::INT3_field(int_3, "Face");

					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
	}
}

//////
// Pipe line
//////

static SHADER::VertexShader vs;
static void smart_load(SubGraphics* sub_graphics)
{
	if (vs.GetShader() == nullptr)
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> input_layout;
		BTYPE::Vertex_default::T_get_input_layout(input_layout);

		vs.initialize(
			sub_graphics->Device,
			L"vs_MeshMesh",
			input_layout
		);
	}
}
void MeshPipe::draw(SubGraphics* sub_graphics, ConstantBuffer<BTYPE::CB_World>& world)
{
	if (is_draw_able() == false) return;

	shader->set_mesh_shader(sub_graphics);
	draw_mesh_only(sub_graphics, world);
}

void MeshPipe::draw_mesh_only(SubGraphics* sub_graphics, ConstantBuffer<BTYPE::CB_World>& world)
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
	mesh->set_mesh_mesh(sub_graphics);
	mesh->draw_mesh_mesh(sub_graphics);
}

void MeshPipe::mesh_pipe_draw_detail()
{
	ImGui::base_field(this->shader, "Shader");
	ImGui::base_field(this->mesh, "Mesh");
}

//////
// MeshRenderer
//////

std::type_index MeshRenderer::get_class_type()
{
	return typeid(MeshRenderer);
}

void MeshRenderer::draw(SubGraphics* sub_graphics)
{
	mesh_pipe.draw(sub_graphics, owner->world);
}

void MeshRenderer::draw_mesh_only(SubGraphics* sub_graphics)
{
	mesh_pipe.draw_mesh_only(sub_graphics, owner->world);
}

#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: 이제 여러 메쉬 타입에 따른 파이프라인을 어떻게 만드는지 이해하겠는데.. 구조를 다시 바꾸기는 어려울 듯 하다...")
#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: 마음에 안들지만 어쩔 수 없지.")
void MeshRenderer::draw_detail_view()
{
	ViaRenderer::draw_detail_view();

	if (ImGui::CollapsingHeader("Mesh renderer"))
	{
		mesh_pipe.mesh_pipe_draw_detail();
	}
}

std::unique_ptr<EDITOR::Editor_add_mesh> eam;
EDITOR::Editor_add_mesh* EDITOR::Editor_add_mesh::get_instance()
{
	if (eam.get() == nullptr)
		eam = std::make_unique<EDITOR::Editor_add_mesh>();
	return eam.get();;
}

void EDITOR::editor_add_mesh(SafePtr<GameObject> parent, SafePtr<MESH::MeshMesh> mesh_mesh)
{
	auto i = EAM::get_instance();
	i->mesh_object = parent->add_child("Mesh object");
	i->mesh_renderer = i->mesh_object->add_component<MeshRenderer>();
	i->mesh_renderer->mesh_pipe.mesh = mesh_mesh;
	i->mesh_renderer->mesh_pipe.shader = mesh_mesh->addition_mm.default_shader;
	i->mesh_renderer->set_visibility(true);
}

std::unique_ptr<EDITOR::Free_view_mesh_mesh_renderer> fvmmr;
EDITOR::Free_view_mesh_mesh_renderer* EDITOR::Free_view_mesh_mesh_renderer::get_instance()
{
	if (fvmmr.get() == nullptr)
		fvmmr = std::make_unique<EDITOR::Free_view_mesh_mesh_renderer>();
	return fvmmr.get();
}

void EDITOR::free_view_mesh_mesh_renderer(SafePtr<MESH::MeshMesh> mesh_mesh)
{
	auto fvmmr = FVMMR::get_instance();
	auto shared = EDITOR::Shared::get_shared();
	
	auto free_view_scene = shared->engine->root_scene.add_child("Free view - " + mesh_mesh->get_name().get_string());
	auto root_game_object = free_view_scene->root_game_object.get_safe_ptr().cast<GameObject>();
	
	// 3d 카메라를 만들고
	EDITOR::editor_add_3d_camera(root_game_object, free_view_scene);
	auto ea3c = EA3C::get_instance();

	// 메쉬를 추가한 후
	EDITOR::editor_add_mesh(root_game_object, mesh_mesh);
	auto eam = EAM::get_instance();

	// 렌더링 되도록 만든다.
	fvmmr->editor_detail_component = root_game_object
		->add_component(new EditorDetailComponent(nullptr));

	unique_ptr<EditorDetailWindow_ImageView> image_view;
	EDITOR::create_image_view_from_deferred_renderer(
		image_view,
		ea3c->render_to_deferred
	);
	image_view->window_name = mesh_mesh->get_name().get_string();

	// Image view의 사이즈가 변할 때 마다 
	//render to deferred의 render target texture의 크기가 변하도록 한다.
	// Delegate를 개선 할 수 있는 가능 성이 있구만...
	RenderToTexture* rtt = ea3c->render_to_deferred.get();
	image_view->delegate_view_size_is_changed
		.AddInvoker(rtt, &RenderToTexture::resize_render_texture);
	image_view->delegate_view_size_is_changed
		.AddInvoker(ea3c->camera_3D.get(), &Camera::change_camera_wh);
	SubGraphics* sg = ea3c->graphics.get();
	image_view->delegate_view_size_is_changed
		.AddInvoker(sg, &SubGraphics::update_viewport_size);

	fvmmr->editor_detail_component->editor_detail_window.reset(image_view.release());
	fvmmr->editor_detail_component->is_draw_detail_window = true;
}

MeshRendererEditorWindow::MeshRendererEditorWindow(SafePtr<MeshRenderer> mesh_renderer) : mesh_renderer(mesh_renderer)
{
}

void MeshRendererEditorWindow::draw_detail_view()
{

}
