#include "Editor_ImageView.h"
#include "../EditorWindow/EditorWindow.h"

using namespace std;

Editor_ImageView::Editor_ImageView(vector<SafePtr<TEXTURE::Texture>> allow_textures) :
	allow_textures(allow_textures)
{
}
Editor_ImageView::~Editor_ImageView() {}

type_index Editor_ImageView::get_class_type()
{
	return typeid(Editor_ImageView);
}

void Editor_ImageView::draw_detail_view()
{
	// 준비
	bool open_state = true;
	string name = this->owner->name;
	name += "##";
	name += StringHelper::ptr_to_string(this);
	// 플레이
	if (ImGui::Begin(name.c_str(), &open_state))
	{
		if (ImGui::InputInt("Index", &this->draw_index))
		{
			if (static_cast<UINT>(this->draw_index) > this->allow_textures.size() - 1)
				this->draw_index = static_cast<int>(this->allow_textures.size() - 1);
			if (static_cast<UINT>(this->draw_index) < 0)
				this->draw_index = 0;
		}

		if (allow_textures[draw_index]->texture.Get() != nullptr)
		{
			auto current_window = ImGui::GetCurrentWindow();
			ImVec2 Size = current_window->Size;
			// Size.x -= 3; // 적절한 조정법이 뭔지 모르겠다. 타이틀 바만큼 제거
			// 조정 포기.
			float min_size = min(Size.x, Size.y);// -10;

			ImGui::Image(allow_textures[draw_index]->GetTextureResourceView(), ImVec2(min_size, min_size));
		}
	}
	ImGui::End();
}

std::type_index EditorDetailWindow_ImageView::get_class_type()
{
	return typeid(EditorDetailWindow_ImageView);
}

void EditorDetailWindow_ImageView::draw_detail_view()
{
	// 플레이
	ImGuiWindowFlags window_flags
		= ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar
		| ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar;
	if (ImGui::Begin(window_name.c_str(), 0, window_flags))
	{
		update_is_selected_window();

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Select display texture"))
			{
				for (auto& elum : allow_textures)
				{
					if (elum.texture.is_vaild() == true)
					{
						if (ImGui::MenuItem(elum.display_name.c_str()))
						{
							this->allow_texture = elum.texture;
						}
					}
					else
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.1f, 0.1f, 1.0f));
						ImGui::MenuItem(elum.display_name.c_str());
						ImGui::PopStyleColor();
					}
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Display mode"))
			{
				bool CENTER_SQUARE		= mode == MODE::CENTER_SQUARE;
				bool FULL_BOX			= mode == MODE::FULL_BOX;
				bool CENTER_SPECIFIC	= mode == MODE::CENTER_SPECIFIC;
				if (ImGui::Checkbox("CENTER_SQUARE", &CENTER_SQUARE))
					mode = MODE::CENTER_SQUARE;
				if (ImGui::Checkbox("FULL_BOX", &FULL_BOX))
					mode = MODE::FULL_BOX;
				if (ImGui::Checkbox("CENTER_SPECIFIC", &CENTER_SPECIFIC))
					mode = MODE::CENTER_SPECIFIC;

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		if (allow_texture.is_vaild() && allow_texture->GetTextureResourceView() != nullptr)
		{
			auto current_window = ImGui::GetCurrentWindow();
			ImVec2 size = current_window->Size;
			// Width는 양옆 페딩만큼 빼줘야 한다.
			// Height에는 타이틀바, 메뉴바, 위 아래 페딩 크기만큼 빼줘야 한다.

			ImVec2 image_size;
			image_size.x 
				= size.x 
				- DEFINE_IMAGE_PADDING * 2.0f;
			image_size.y 
				= size.y 
				- ImGui::GetCurrentWindow()->TitleBarHeight()
				- ImGui::GetCurrentWindow()->MenuBarHeight()
				- DEFINE_IMAGE_PADDING * 2.0f;
			switch (mode) 
			{
			case MODE::CENTER_SQUARE:
			{
				float min_size = min(image_size.x, image_size.y);// -10;
				image_size.x = min_size; image_size.y = min_size;
			}
				break;
			case MODE::FULL_BOX:
				break;
			case MODE::CENTER_SPECIFIC:
				break;
			}

			// delegate call
			if (this->width != image_size.x || this->height != image_size.y)
			{
				this->width = static_cast<UINT>(image_size.x);
				this->height = static_cast<UINT>(image_size.y);

				delegate_view_size_is_changed.Invoke(width, height);
			}

			switch (mode)
			{
			case MODE::CENTER_SQUARE:
				ImGui::SetCursorPos(
					ImVec2(
						size.x / 2.0f - image_size.x / 2.0f,
						DEFINE_IMAGE_PADDING
						+ ImGui::GetCurrentWindow()->TitleBarHeight()
						+ ImGui::GetCurrentWindow()->MenuBarHeight()
					));
				ImGui::Image(allow_texture->GetTextureResourceView(), image_size);
			break;
			case MODE::FULL_BOX:
				//ImGui::SetCursorPos(ImVec2(
				//	-50.0f,
				//	ImGui::GetCurrentWindow()->TitleBarHeight()
				//	+ ImGui::GetCurrentWindow()->MenuBarHeight()
				//));
				ImGui::Image(allow_texture->GetTextureResourceView(), image_size);
			break;
			case MODE::CENTER_SPECIFIC:
				// 현재로서 지원하지 않음.
				ImGui::Text("It is not supported yet.");
				break;
			}
		}
		ImGui::End();
	}
}

void EditorDetailWindow_ImageView::detail_window_detail_view()
{
	ImGui::Text("It is editor detail window image view");
}

unique_ptr<EDITOR::Defered_renderer_to_image_view> drtiv;
EDITOR::Defered_renderer_to_image_view* EDITOR::Defered_renderer_to_image_view::get_instance()
{
	if (drtiv.get() != nullptr)
		drtiv = make_unique<EDITOR::Defered_renderer_to_image_view>();
	return drtiv.get();
}

void EDITOR::create_image_view_from_deferred_renderer(std::unique_ptr<EditorDetailWindow_ImageView>& edwiv, SafePtr<RenderToDeferred> rtd)
{
	if (edwiv.get() == nullptr)
		edwiv = make_unique<EditorDetailWindow_ImageView>();

	edwiv->allow_texture = rtd->render_target_texture_0.get();

	edwiv->allow_textures.emplace_back(
		"Base color",
		rtd->render_target_texture_0.get()
	);
	edwiv->allow_textures.emplace_back(
		"Normal",
		rtd->render_target_texture_1.get()
	);
	edwiv->allow_textures.emplace_back(
		"Result color",
		rtd->render_target_texture_2.get()
	);
	edwiv->allow_textures.emplace_back(
		"Light weight",
		rtd->render_target_texture_3.get()
	);
	edwiv->allow_textures.emplace_back(
		"World position",
		rtd->render_target_texture_4.get()
	);
	edwiv->allow_textures.emplace_back(
		"Depth",
		rtd->render_target_texture_5.get()
	);

	//auto cew = EDITOR::Create_editor_window::get_instance();
	//edwiv->invoke_controll(cew->display->renderWindow.get());
}
