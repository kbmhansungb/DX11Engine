#pragma once

#include "../../Component/_Exclude.h"
#include "../../Game/ResourceImporter.h"

#define DEFINE_START_WINDOW_WIDTH	1600
#define DEFINE_START_WINDOW_HEIGHT	900
#define DEFINE_IMAGE_PADDING		8.0f
class EditorDetailWindow_ImageView;

namespace EDITOR
{
	class Create_editor_window : public Base
	{
	public:

		class EditorSetting
		{
		public:
			bool view_project_setting = false;
			bool view_pipelineing = false;
			bool view_content_viewer = false;
			bool view_hierarchy = false;
			bool view_editor_scene = false;
			bool view_detail = false;

			template <typename PayloadType>
			void serialize(PayloadType* payload)
			{
				payload->with(&view_project_setting);
				payload->with(&view_pipelineing);
				payload->with(&view_content_viewer);
				payload->with(&view_hierarchy);
				payload->with(&view_editor_scene);
				payload->with(&view_detail);
			}
		};

		RESOURCE_IMPORT_SYSTEM::ResourceImporter* now_selected_importer;

		EditorSetting				editor_setting;

		// other
		SafePtr<Engine>				engine;
		SafePtr<Display>			display;
		SafePtr<RenderToSwapChain>	swap_chain_render_target;

		// camera
		SafePtr<GameObject>			camera_object;
		SafePtr<Camera>				camera_3d;
		SafePtr<ImGuiGraphics>		ImGui_graphics;

		SafePtr<Base>				last_select;

		std::unique_ptr<EditorDetailWindow_ImageView> edwiv;

		static Create_editor_window* get_instance();

		Create_editor_window();
		virtual ~Create_editor_window();
		// Base을(를) 통해 상속됨
		virtual void draw_detail_view() override;

	private:
		void set_ImGui();
		void draw_project_setting();
		void draw_pipelineing();
		void draw_content_viewr();
		void draw_hierarchy_scene(Scene* scene);
		void draw_hierarchy_game_object(Scene* scene);
		void draw_hierarchy_game_object(GameObject* game_object);
		void draw_hierarchy();
		void draw_view_detail();
		void draw_file_browser();
	public:
		ImGui::FileBrowser file_browser = ImGui::FileBrowser(
			ImGuiFileBrowserFlags_::ImGuiFileBrowserFlags_MultipleSelection
		);
	};
	typedef Create_editor_window CEW;

	extern void create_editor_window(SafePtr<Engine> engine, SafePtr<Scene> main_scene);
}

class EditorComponent
{
public:
};

namespace ImGui
{
	template <typename Type>
	void CheckBox_for_kown_select(Type* type)
	{
		auto& I = *EDITOR::CEW::get_instance();
		static float offset = 2.0f;

		ImGuiContext& context = *ImGui::GetCurrentContext();
		ImGuiWindow& window = *ImGui::GetCurrentWindow();
		float backup = context.FontSize;
		context.FontSize = 8.0f;
		window.DC.CursorPos.x += offset;
		window.DC.CursorPos.y += offset;

		// 선택 버튼에 대해서 정의
		bool is_selected = type == I.last_select.get();
		std::string name = "##"; name += StringHelper::ptr_to_string(type);
		if (ImGui::Checkbox(name.c_str(), &is_selected))
		{
			if (I.last_select.get() != type)
				I.last_select = type;
			else
				I.last_select = SafePtr<Base>();
		}
		ImGui::PushItemWidth(0);
		ImGui::SameLine();

		window.DC.CursorPos.x -= offset;
		window.DC.CursorPos.y -= offset;
		context.FontSize = backup;
	}
}

