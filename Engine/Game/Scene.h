#pragma once

#include "_Include.h"
#include "GameObject.h"
class Engine;

class Scene final : 
	public Base, 
	public RendererContainer,
	public LightingContainer
{
public:
	Scene();
	Scene(const std::string name, SafePtr<Engine> engine);
	Scene(const std::string name, SafePtr<Scene> parent);
	virtual ~Scene() {};
	virtual std::type_index get_class_type() { return typeid(Scene); }

public:

private:	bool _active = true;
public:		__declspec(property(get = get_active, put = put_active)) bool active;
	  bool get_active() { return _active; }
	  void put_active(bool active) { _active = active; }

private:	SafePtr<Engine> _this_engine;
public:		SafePtr<Engine> get_engine() { return _this_engine; }
			__declspec(property(get = get_engine)) SafePtr<Engine> this_engine;

private:	SafePtr<Scene> _parent;
public:		SafePtr<Scene> get_parent() { return _parent; }
			__declspec(property(get = get_parent)) SafePtr<Scene> parent;
			SafePtr<Scene> add_child(const std::string name);
			void remove_child(SafePtr<Scene> scene);

private:	std::list<std::unique_ptr<Scene>> _childs;
public:		const std::list<std::unique_ptr<Scene>>& get_childs_array() { return _childs; }
			
	std::string name;
	GameObject root_game_object;

private:
	void update();
	friend class Engine;

public:
	virtual void draw_detail_view() override;
};