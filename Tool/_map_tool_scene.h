//#pragma once
//
//#include "_map_tool_scene.h"
//
//#include "ToolMain_ImGui.h"
//#include "_map_tool_ImGui.h"
//
//namespace MapTool
//{
//	static void build_scene()
//	{
//		auto Data = ToolData::get_map_tool_data();
//
//		Data->Tool_main_ImGui = Data->root_game_object->add_component<ToolMainImGui>(make_unique<ToolMainImGui>());
//		Data->Tool_main_ImGui->PutActiveState(true);
//		Data->Tool_setting_ImGui = Data->root_game_object->add_component<ToolSettingImGui>(make_unique<ToolSettingImGui>());
//		Data->Tool_setting_ImGui->PutActiveState(true);
//		{
//			// 맵툴 생성
//			Data->map_tool_ImGui = Data->root_game_object->add_component<MapToolImGui>(
//				make_unique<MapToolImGui>());
//			Data->map_tool_ImGui->PutActiveState(true);
//			Data->map_too_behavior = Data->root_game_object->add_component<MapToolBehavior>(
//				make_unique<MapToolBehavior>());
//			Data->map_too_behavior->PutActiveState(true);
//		}
//
//		// 초기화
//		Data->terrain_renderer.get()->PutActiveState(false);
//	}
//}