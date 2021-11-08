//#pragma once
//
//#include "_load_resource.h"
//
//#include "_model_tool_component.h"
//#include "_model_tool_ImGui.h"
//
//namespace ModelTool
//{
//	static void  build_scene()
//	{
//		auto Data = ToolData::get_map_tool_data();
//
//
//		Data->model_renderer = Data->root_game_object
//			->add_component<ModelRenderer>(
//				make_unique<ModelRenderer>()
//				);
//		Data->model_renderer->Render_target_model
//			.set(Data->engine->get_resource(Data->engine->make_name("untitled.fbx")));
//
//		auto Model_tool_ImGui = Data->root_game_object
//			->add_component<ModelToolImGui>(
//				make_unique<ModelToolImGui>()
//				);
//		Model_tool_ImGui->Model_renderer.set(Data->model_renderer);
//		Model_tool_ImGui->PutActiveState(true);
//		
//		Data->model_renderer->PutActiveState(false);
//
//
//	}
//}
