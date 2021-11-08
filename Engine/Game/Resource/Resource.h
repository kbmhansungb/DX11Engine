#pragma once

#include "_Include.h"

class Engine;
template <typename friend_class> class Resources;

class Resource : public Base
{
	friend Resources<Engine>;
protected:
	Name Registed_name;
public:
	Resource();
	virtual ~Resource();
	virtual std::type_index get_class_type();
	
private: bool _loaded = false;
public: __declspec (property (get = get_loaded)) bool loaded;
	bool get_loaded() { return _loaded; }

	virtual void load_resource(Engine* engine) { _loaded = true; };
	virtual void release_resource(Engine* engine) { _loaded = false; };

	Name get_name();
	virtual void draw_detail_view() override;

	void rename(Engine* engine, std::string new_name);
};