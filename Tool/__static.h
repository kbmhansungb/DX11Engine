//#pragma once
//#include "__Include.h"
//
//class ToolMainImGui;
//class ToolSettingImGui;
//class MapToolImGui;
//class MapToolBehavior;
//class PlaneMeshTool;
//
//class TerrainRenderer;
//class TerrainShader;
//
//class ModelRenderer;
//
//class ToolData
//{
//public:
//	// editor default
//	SafePtr<Engine> engine;
//	int window_width;
//	int window_height;
//
//	// Editor game object and component
//	SafePtr<ToolMainImGui> Tool_main_ImGui;
//	SafePtr<ToolSettingImGui> Tool_setting_ImGui;
//
//	SafePtr<Scene> root_scene;
//	SafePtr<GameObject> root_game_object;
//
//	SafePtr<Display> main_display;
//	SafePtr<WindowInputContainer> window_input_container;
//	SafePtr<SwapchainRenderTarget> render_target;
//	bool extend_map = false;
//	vector<int> now_selected_index = vector<int>({ 0, 0 });
//
//	SafePtr<GameObject> camera_object;
//	SafePtr<Camera3D> camera_3D;
//	SafePtr<GraphicsWithImGui> graphics_with_ImGui;
//	SafePtr<SimpleMoveTransform> simple_move_transform;
//
//	// Model
//	SafePtr<ModelRenderer> model_renderer;
//
//	// Terrain
//	SafePtr<GameObject> game_object_map;
//	SafePtr<TerrainShader> terrain_material;
//	SafePtr<TerrainRenderer> terrain_renderer;
//	SafePtr<PlaneMeshTool> plane_mesh_tool;
//	SafePtr<DirectRenderer> line_renderer;
//
//	// Editor data
//	vector<string> models;
//	vector<const char*> model_names;
//	vector<string> brushes;
//	vector<const char*> brush_names;
//	vector<string> textures;
//	vector<const char*> texture_names;
//
//	SafePtr<MapToolImGui> map_tool_ImGui;
//	SafePtr<MapToolBehavior> map_too_behavior;
//
//	static ToolData* get_map_tool_data();
//};