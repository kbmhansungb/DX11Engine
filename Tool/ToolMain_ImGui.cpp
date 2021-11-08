//#include "ToolMain_ImGui.h"
//
//#include "_map_tool_component.h"
//#include "_model_tool_component.h"
//
//void ToolMainImGui::draw(SubGraphics* Sub_graphics)
//{
//	ImVec2 WindowPos = ImGui::GetWindowPos();
//	WindowPos.x -= 55;
//	WindowPos.y -= 55;
//	ImGui::SetNextWindowPos(WindowPos);
//	ImVec2 WindowSize = ImGui::GetWindowSize();
//	WindowSize.x *= 4 - 5;
//	WindowSize.y *= 4 - 5;
//	ImGui::SetNextWindowSize(WindowSize);
//
//	ImGuiWindowFlags Window_flags = 0;
//	Window_flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoMove;
//	Window_flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoResize;
//	Window_flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground;
//	Window_flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar;
//	Window_flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar;
//	Window_flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoDocking;
//	ImGui::Begin("Tool set", 0, Window_flags);
//	{
//		if (ImGui::Button("Model tool"))
//		{
//			if (State == TOOLMAINGUISTATE::TOOLMAINGUISTATE_MODEL_TOOL)
//			{
//				set_state(TOOLMAINGUISTATE::TOOLMAINGUISTATE_NOTTHING);
//			}
//			else
//			{
//				set_state(TOOLMAINGUISTATE::TOOLMAINGUISTATE_MODEL_TOOL);
//			}
//		}
//		ImGui::SameLine();
//		if (ImGui::Button("Map tool"))
//		{
//			if (State == TOOLMAINGUISTATE::TOOLMAINGUISTATE_MAP_TOOL)
//			{
//				set_state(TOOLMAINGUISTATE::TOOLMAINGUISTATE_NOTTHING);
//			}
//			else
//			{
//				set_state(TOOLMAINGUISTATE::TOOLMAINGUISTATE_MAP_TOOL);
//			}
//		}
//
//		ImGui::Checkbox("Tool set setting", &Tool_setting);
//	}
//	ImGui::End();
//}
//
//void ToolMainImGui::set_state(TOOLMAINGUISTATE New_state)
//{
//	auto Data = ToolData::get_map_tool_data();
//	switch (this->State)
//	{
//	case TOOLMAINGUISTATE::TOOLMAINGUISTATE_MODEL_TOOL:
//		Data->model_renderer->PutActiveState(false);
//		break;
//	case TOOLMAINGUISTATE::TOOLMAINGUISTATE_MAP_TOOL:
//		Data->terrain_renderer->PutActiveState(false);
//		break;
//	}
//	switch (New_state)
//	{
//	case TOOLMAINGUISTATE::TOOLMAINGUISTATE_MODEL_TOOL:
//		Data->model_renderer->PutActiveState(true);
//		break;
//	case TOOLMAINGUISTATE::TOOLMAINGUISTATE_MAP_TOOL:
//		Data->terrain_renderer->PutActiveState(true);
//		break;
//	}
//	this->State = New_state;
//}
//
//void ToolSettingImGui::draw(SubGraphics* Sub_graphics)
//{
//	auto Data = ToolData::get_map_tool_data();
//
//	if (Data->Tool_main_ImGui->Tool_setting)
//	{
//		ImGuiWindowFlags Window_flags = 0;
//		Window_flags |= ImGuiWindowFlags_::ImGuiWindowFlags_HorizontalScrollbar;
//		ImGui::Begin("Tool setting");
//
//		Data->camera_3D->draw_imgui();
//		Data->simple_move_transform->draw_imgui();
//
//		// 리소스 보기
//		Resources* result = static_cast<Resources*>(Data->engine.get());
//		if(result != nullptr)
//			result->draw_ImGui();
//
//		ImGui::End();
//	}
//}
//
