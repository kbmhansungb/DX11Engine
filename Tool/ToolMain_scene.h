//#pragma once
//#include "__static.h"
//#include "_load_resource.h"
//#include "_map_tool_scene.h"
//#include "_model_tool_scene.h"
//
//namespace MainTool
//{
//	static void build_scene(Engine* engine)
//
//		// 랜더 오브젝트 생성
//		{
//			Data->game_object_map = Data->root_game_object->add_child("Render object");
//
//			// 랜더러 생성
//			Data->terrain_renderer = Data->game_object_map->add_component<TerrainRenderer>(make_unique<TerrainRenderer>());
//			Data->terrain_renderer->VS_PS_Shader.set(Data->engine->get_resource(Data->engine->make_name(MapToolName::terrain_material)));
//			Data->terrain_renderer->create_square_plane({ 0, 0 });
//			//Data->terrain_renderer->set_decal_texture(
//			//	Data->engine->get_resource(Data->engine->make_name(MapToolName::brush_0))
//			//	.cast<TEXTURE::Image>());
//			Data->terrain_renderer.get()->PutActiveState(true);
//		}
//
//		// line drawer 생성
//		{
//			Data->line_renderer = Data->game_object_map->add_component<DirectRenderer>(make_unique<DirectRenderer>());
//			auto& Mesh = Data->line_renderer->VS_PS_have_mesh;
//
//			auto have_mesh = Data->engine->add_resource<MESH::VS_IB_Have_Mesh<BTYPE::VB_MeshVertex, BTYPE::IB_Default>>
//				(
//					Data->engine->make_name(MapToolName::line_mesh),
//					make_unique< MESH::VS_IB_Have_Mesh<BTYPE::VB_MeshVertex, BTYPE::IB_Default>>()
//					);
//			Data->line_renderer->VS_PS_have_mesh.set(have_mesh);
//
//			Mesh->Vertex_vector.clear();
//			Mesh->Vertex_vector.emplace_back(0, 0, 0, 0, 0, 0, 0, 0);
//			Mesh->Vertex_vector.emplace_back(0, 0, 0, 0, 0, 0, 0, 0);
//			Mesh->Index_vector = { 0, 1 };
//			Data->line_renderer->VS_PS_Shader.set(Data->engine->get_resource(Data->engine->make_name(MapToolName::line_material)));
//			Data->line_renderer.get()->PutActiveState(true);
//		}
//		
//		// Child tool 생성.
//		MapTool::build_scene();
//		ModelTool::build_scene();
//	}
//};