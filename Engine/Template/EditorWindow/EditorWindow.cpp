#include "EditorWindow.h"
#include "EditorDefineContainer.h"

#include "../../ImGui/imgui_internal.h"
#include "../Editor_FreeView/Editor_FreeView.h"
#include "../Editor_ImageView/Editor_ImageView.h"

using namespace std;

void EDITOR::Create_editor_window::draw_project_setting()
{
	ImGui::Begin("Project setting");
	{
		ImGui::Text("Text");
	}
	ImGui::End();
}

void EDITOR::Create_editor_window::draw_pipelineing()
{
	ImGui::Begin("Pipelineing");
	{
		ImGui::Text("Text");
	}
	ImGui::End();
}

void EDITOR::Create_editor_window::draw_content_viewr()
{
	static Resources<Engine>* resources = nullptr;
	if (resources == nullptr) resources = &engine->resources;

	ImGui::Begin(
		"Content viewer", 
		&this->editor_setting.view_content_viewer,
		ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar
	);
	{
		if (ImGui::BeginPopup("Add resource popup"))
		{
			static string text_field = "";
			ImGui::Text_field(text_field, "Type", 100);

			auto& resource_map = EDITOR::Define::get_define()->resource_construct_map;
			for (auto iter = resource_map.begin(); iter != resource_map.end(); ++iter)
			{
				if (iter->first.find(text_field) != string::npos)
				{
					// 해당하는 스트링이 있는 경우에만.
					if (ImGui::Button(iter->first.c_str(), ImVec2(-1, 0)))
					{
						// 중복된 이름을 허용하지 않음.
						int index = 0;
						std::string resource_name = iter->first + "_" + to_string(index);
						Name name = Name(&this->engine->name_container, resource_name);
						while (this->engine->resources.get_resource(name).is_vaild())
						{
							resource_name = iter->first + "_" + to_string(index);
							name = Name(&this->engine->name_container, resource_name);

							++index;
						}
						// 버튼이 눌리면 리소스 추가
						iter->second->add_resource(resource_name, this->engine.get());
						ImGui::CloseCurrentPopup();
					}
				}
			}

			ImGui::EndPopup();
		}

		// 어이없네.. Menu에서는 왜 OpenPopup이 안되는 것인가.
		// Menu frame 안에서 팝업이 열리는 건가??.
		bool Add_resource = false;
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Content menu"))
			{
				if (ImGui::MenuItem("Add resource")) Add_resource = true;
				
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		if (Add_resource) ImGui::OpenPopup("Add resource popup");

		if (resources != nullptr)
			for (auto& pair : resources->Resource_map)
			{
				ImGui::CheckBox_for_kown_select(pair.second.get());
				if (ImGui::TreeNode(pair.second->get_name().get_string().c_str()))
				{
					ImGui::TreePop();
				}
			}
	}
	ImGui::End();
}

void EDITOR::Create_editor_window::draw_hierarchy_scene(Scene* scene)
{
	// Scene을 그리고
	ImGui::CheckBox_for_kown_select(scene);
	if (ImGui::TreeNode((scene->name).c_str()))
	{
		// 파생전파
		for (const auto& child : scene->get_childs_array())
		{
			draw_hierarchy_scene(child.get());
		}

		// Popup
		ImGui::TreePop();
	}
}

void EDITOR::Create_editor_window::draw_hierarchy_game_object(Scene* scene)
{

	draw_hierarchy_game_object(&scene->root_game_object);

	for (const auto& child : scene->get_childs_array())
	{
		draw_hierarchy_game_object(child.get());
	}
}

void EDITOR::Create_editor_window::draw_hierarchy_game_object(GameObject* game_object)
{
	ImGui::CheckBox_for_kown_select(game_object);
	if (ImGui::TreeNode(game_object->name.c_str()))
	{
		for (auto& child : game_object->childs)
		{
			draw_hierarchy_game_object(child.get());
		}
		ImGui::TreePop();
	}
}

void EDITOR::Create_editor_window::draw_hierarchy()
{
	ImGui::Begin("Hierarchy");
	{
		if (ImGui::CollapsingHeader("Scene"))
		{
			draw_hierarchy_scene(&engine->root_scene);
		}
		if (ImGui::CollapsingHeader("Game objects"))
		{
			draw_hierarchy_game_object(&engine->root_scene);
		}
	}
	ImGui::End();
}

void EDITOR::Create_editor_window::draw_view_detail()
{
	ImGui::Begin("Detail");
	{
		if (last_select.is_vaild()) last_select->draw_detail_view();
	}
	ImGui::End();
}

void EDITOR::Create_editor_window::draw_file_browser()
{
	file_browser.Display();

	if (file_browser.HasSelected()) // 버튼의 클릭과 같다.
	{
		auto paths = file_browser.GetMultiSelected();
		
		for (size_t index = 0; index < paths.size(); ++index)
		{
			string path = paths[index].string();
			now_selected_importer->import_file(this->engine.get(), path);
		}

		file_browser.ClearSelected();
		file_browser.Close();
	}
}

unique_ptr<EDITOR::Create_editor_window> cew;

EDITOR::Create_editor_window* EDITOR::Create_editor_window::get_instance()
{
	if (cew.get() == nullptr)
		cew = make_unique<EDITOR::Create_editor_window>();
	return cew.get();
}

void EDITOR::create_editor_window(SafePtr<Engine> engine, SafePtr<Scene> main_scene)
{
	auto I = CEW::get_instance();
	I->engine = engine;

	auto payload = BinaryLoad::BinaryLoadPayload("Setting.proj");
	if (payload.is_open()) I->editor_setting.serialize(&payload), payload.close();

	auto shared = EDITOR::Shared::get_shared();

	I->display = engine->root_scene.root_game_object.add_component(
		new Display(
			SafePtr<Display>(),
			L"Kim`s playground",
			L"Basic",
			DEFINE_START_WINDOW_WIDTH,
			DEFINE_START_WINDOW_HEIGHT,
			true)
	);
	I->display->active = true;

	I->swap_chain_render_target = engine->root_scene.root_game_object.add_component(
		new RenderToSwapChain(I->display)
	);
	//I->swap_chain_render_target->Clear_color = XMFLOAT4(0.13f, 0.13f, 0.13f, 0.13f);
	I->swap_chain_render_target->active = true;
	
	auto ea3c = EA3C::get_instance();
	// 카메라를 만든후 윈도우를 등록해야 한다.
	EDITOR::editor_add_3d_camera(
		&engine->root_scene.root_game_object, 
		main_scene
	);
	I->camera_object = ea3c->camera_object;
	I->camera_3d = ea3c->camera_3D;
	// simple move 추가
	ea3c->camera_object->add_component(new SimpleMoveTransform(I->display))->active = true;

	EDITOR::create_image_view_from_deferred_renderer(I->edwiv, ea3c->render_to_deferred);
	//I->edwiv->invoke_controll(I->display->renderWindow.get());

	I->edwiv->window_name = "Editor scene";
	// Delegate 바인딩.
	// 1. 이미지 크기 변경. (렌더 타겟 텍스쳐, 뎁스 크기, 등..)
	// 2. 카메라 크기 변경.
	// 3. 뷰포트 크기 변경.
	I->edwiv->delegate_view_size_is_changed
		.AddInvoker(ea3c->render_to_deferred.get(), &RenderToTexture::resize_render_texture);
	I->edwiv->delegate_view_size_is_changed
		.AddInvoker(ea3c->camera_3D.get(), &Camera::change_camera_wh);
	I->edwiv->delegate_view_size_is_changed
		.AddInvoker(ea3c->graphics.get(), &SubGraphics::update_viewport_size);

	I->ImGui_graphics = engine->root_scene.root_game_object.add_component(
		new ImGuiGraphics(
			ea3c->camera_3D.cast<Camera>(),
			I->swap_chain_render_target,
			DEFINE_START_WINDOW_WIDTH,
			DEFINE_START_WINDOW_HEIGHT)
	);
	I->ImGui_graphics->ImGui_draw_list.push_back(cew.get());
	I->ImGui_graphics->active = true;

	// Editor scene 등록.
	if (I->editor_setting.view_editor_scene)
		I->ImGui_graphics->ImGui_draw_list.push_back(I->edwiv.get());
}

EDITOR::Create_editor_window::Create_editor_window()
{
	auto payload = BinaryLoad::BinaryLoadPayload("Setting.proj");
	if (payload.is_open()) editor_setting.serialize(&payload), payload.close();
}

EDITOR::Create_editor_window::~Create_editor_window()
{
	auto payload = BinarySave::BinarySavePayload("Setting.proj");
	if (payload.is_open()) editor_setting.serialize(&payload), payload.close();
}

std::type_index EDITOR::Create_editor_window::get_class_type()
{
	return typeid(Create_editor_window);
}

void EDITOR::Create_editor_window::draw_detail_view()
{
	set_ImGui();

	//////
	// Menu

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Editor"))
		{
			if (ImGui::BeginMenu("Import"))
			{
				auto importer = RESOURCE_IMPORT_SYSTEM::ResourceImportSystem::get_resource_import_system();
				for (auto iter = importer->resource_importer.begin(); iter != importer->resource_importer.end(); ++iter)
				{
					if (ImGui::MenuItem(iter->first.c_str()))
					{
						now_selected_importer = iter->second;
						file_browser.Open();
						file_browser.GetMultiSelected();
						file_browser.SetTitle("Importer");
						file_browser.SetTypeFilters({ ".*" });
					}
				}
				ImGui::EndMenu();
			}

			//if (ImGui::MenuItem("Close"))
			//{
			//}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			ImGui::Checkbox("Project setting", &editor_setting.view_project_setting);
			ImGui::Checkbox("Pipelineing", &editor_setting.view_pipelineing);
			ImGui::Checkbox("Content viewer", &editor_setting.view_content_viewer);
			ImGui::Checkbox("Hierarchy", &editor_setting.view_hierarchy);
			if (ImGui::Checkbox("Editor scene", &editor_setting.view_editor_scene))
			{
				if (editor_setting.view_editor_scene)
				{
					this->ImGui_graphics->ImGui_draw_list
						.push_back(this->edwiv.get());
				}
				else
				{
					this->ImGui_graphics->ImGui_draw_list
						.remove(this->edwiv.get());
				}
			}
			ImGui::Checkbox("Detail", &editor_setting.view_detail);
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	//////
	// function
	if (editor_setting.view_project_setting)	draw_project_setting();
	if (editor_setting.view_pipelineing)		draw_pipelineing();
	if (editor_setting.view_content_viewer)		draw_content_viewr();
	if (editor_setting.view_hierarchy)			draw_hierarchy();
	if (editor_setting.view_detail)				draw_view_detail();

	draw_file_browser();
}

void EDITOR::Create_editor_window::set_ImGui()
{
	ImGui::DockSpaceOverViewport();
}