#include "Engine.h"

Engine::Engine(HINSTANCE hIns) :
	Base(),
	EngineContext(),
	_hInstance(hIns),
	root_scene("root_scene", this)
{
	//EngineContext::initialize();
	timer.Start();
}

void Engine::LoopCall()
{
	while (_engine_activation == true)
	{
		COROUTINE::step();
		
		this->timer.make_delta_time();
		this->root_scene.update();
	}
}

ImGui::base_field_instance bfi;
ImGui::base_field_instance* ImGui::get_base_field_instance()
{
	return &bfi;
}

Component* last_selected_component;
Component* ImGui::get_selected_component()
{
	return last_selected_component;
}
void base_field_component_component(bool(*func)(Base* b), Component* component)
{
	if (func(component) == false) return;

	std::string label 
		= component->owner->name
		+ " - "
		+ typeid(*component).name()
		+ "##"
		+ StringHelper::ptr_to_string(component);
	bool is_selected = false;
	ImGui::Selectable(label.c_str(), &is_selected);
	if (is_selected)
	{
		last_selected_component = component;
	}

}
void base_field_component_object(bool(*func)(Base* b), GameObject* game_object)
{
	for (auto& component : game_object->components)
	{
		base_field_component_component(func, component.get());
	}

	for (auto& child : game_object->childs)
	{
		base_field_component_object(func, child.get());
	}
}
void base_field_component_scene(bool(*func)(Base* b), Scene* scene)
{
	base_field_component_object(func, &scene->root_game_object);

	for (auto& child : scene->get_childs_array())
	{
		base_field_component_scene(func, child.get());
	}
}
void ImGui::base_field_component_start(bool(*func)(Base* b))
{
	auto shared = EDITOR::Shared::get_shared();

	last_selected_component = nullptr;
	base_field_component_scene(func, &shared->engine->root_scene);
}

void base_field_game_object_game_object(GameObject*& result, GameObject& game_object)
{
	std::string label
		= game_object.name
		+ "##"
		+ StringHelper::ptr_to_string(&game_object);
	bool is_selected = false;
	ImGui::Selectable(label.c_str(), &is_selected);
	if (is_selected)
	{
		result = &game_object;
	}

	for (auto& child : game_object.childs)
	{
		base_field_game_object_game_object(result, *child.get());
	}
}

void base_field_game_object_scene(GameObject*& result, Scene& scene)
{
	base_field_game_object_game_object(result, scene.root_game_object);

	for (auto& child : scene.get_childs_array())
	{
		base_field_game_object_scene(result, *child.get());
	}
}

void ImGui::base_field_game_object_start(GameObject*& result)
{
	auto shared = EDITOR::Shared::get_shared();
	base_field_game_object_scene(result, shared->engine->root_scene);
}

void base_field_scene_scene(Scene*& result, Scene& node)
{
	std::string label
		= node.name;
	bool is_selected = false;
	ImGui::Selectable(label.c_str(), &is_selected);
	if (is_selected)
	{
		result = &node;
	}

	for (auto& child : node.get_childs_array())
	{
		base_field_scene_scene(result, *child.get());
	}
}
void ImGui::base_field_scene_start(Scene*& scene)
{
	auto shared = EDITOR::Shared::get_shared();
	base_field_scene_scene(scene, shared->engine->root_scene);
}
