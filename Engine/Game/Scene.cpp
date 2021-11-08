#include "Scene.h"
#include "../Game/Engine.h"

using namespace std;

Scene::Scene() :
	root_game_object()
{
}

Scene::Scene(const string name, SafePtr<Engine> engine) :
	name(name),
	_this_engine(engine),
	_parent(SafePtr<Scene>()),
	root_game_object(this, name + " - game object", SafePtr<GameObject>())
{
}

Scene::Scene(const string name, SafePtr<Scene> parent) :
	name(name),
	_this_engine(parent->this_engine),
	_parent(parent),
	root_game_object(this, name + " - game object", SafePtr<GameObject>())
{
}

SafePtr<Scene> Scene::add_child(const string name)
{
	_childs.emplace_back( new Scene(name, this));
	return _childs.back()->get_safe_ptr().cast<Scene>();
}

void Scene::remove_child(SafePtr<Scene> scene)
{
	// SafePtr은 PtrNode안에서만 사용할 수 있다.
	Scene* scene_ptr = scene.get();
	auto find_result = find_if(_childs.begin(), _childs.end(), [scene_ptr](unique_ptr<Scene>& child)
		{
			return child.get() == scene_ptr;
		});
	if (find_result != _childs.end())
		_childs.erase(find_result);
}

void Scene::update()
{
	for (auto& child_scene : _childs)
	{
		if (child_scene->active) 
			child_scene->update();
	}

	if (root_game_object.active) 
		root_game_object.update();
}

void Scene::draw_detail_view()
{
	Base::draw_detail_view();

	if (ImGui::CollapsingHeader("Scene"))
	{
		std::string new_name = name;
		ImGui::Text_field(new_name, "name", 100);
		if (new_name != name)
		{
			name = new_name;
			root_game_object.name = name + " - game object";
		}

		ImGui::NewLine();
		if (_parent.get() != nullptr)
			// root scene은 삭제할 수 없음.
			if (ImGui::RightAngleButton("Delete"))
			{
				this->_parent->remove_child(this->get_safe_ptr().cast<Scene>());
				return;
			}

		ImGui::NewLine();
		if (ImGui::RightAngleButton("Add child"))
		{
			this->add_child("Child scene");
		}
	}
}