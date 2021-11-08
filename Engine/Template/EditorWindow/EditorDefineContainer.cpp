#include "EditorDefineContainer.h"
#include "../EditorWindow/EditorWindow.h"
#include <memory>

using namespace std;

unique_ptr<EDITOR::Shared> shared;

EDITOR::Shared* EDITOR::Shared::get_shared()
{
	if (shared.get() == nullptr)
	{
		shared = make_unique<EDITOR::Shared>();
	}
	return shared.get();
}

void EDITOR::change_last_select(Base* base)
{
	EDITOR::CEW::get_instance()->last_select = base;
}

EDITOR::ComponentConstruct::ComponentConstruct(std::string regist_name)
{
	Define::get_define()->component_construct_map[regist_name] = this;
}

void EDITOR::ComponentConstruct::add_component_to(GameObject* game_object)
{
	game_object->add_component(create_componenet());
}

EDITOR::ResourceConstruct::ResourceConstruct(std::string regist_name)
{
	Define::get_define()->resource_construct_map[regist_name] = this;
}

void EDITOR::ResourceConstruct::add_resource(std::string resource_name, Engine* engine)
{
	unique_ptr<Resource> resource;
	create_resource(resource);

	engine->resources.add_resource(
		NAME::Name(&engine->name_container, resource_name),
		resource
	);
}

unique_ptr<EDITOR::Define> define;

EDITOR::Define* EDITOR::Define::get_define()
{
	if (define.get() == nullptr)
	{
		define = make_unique<EDITOR::Define>();
	}
	return define.get();
}
