#pragma once

#include "_Include.h"

class Component : public Base
{
public:
friend GameObject;
	Component(Component&) = delete;
	Component(Component&&) = delete;
	Component();
	virtual ~Component();
	virtual void response_update_world() {}

private:	bool _active;
public:		__declspec(property(get = get_active, put = put_active)) bool active;
	  bool get_active() { return _active; }
	  void put_active(bool active) 
	  {
		  if (_active != active)
		  {
			  _active = active;

			  if (active)	awake();
			  else			sleep();
		  }
	  }

	virtual void awake() {}
	virtual void sleep() {}
	virtual void update() {}

private:	SafePtr<GameObject> _owner;
public:		SafePtr<GameObject> get_owner() { return _owner; }
			__declspec(property(get = get_owner)) SafePtr<GameObject> owner;

	virtual void draw_detail_view();
};