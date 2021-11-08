#pragma once

#include <string>
#include <typeinfo>
#include <typeindex> 
#include <map>
#include <memory>

class Base;
class Engine;
class Component;
class GameObject;
class Resource;

namespace EDITOR
{
	struct Shared
	{
		Engine* engine;

		static Shared* get_shared();
	};

	struct ComponentConstruct
	{
		ComponentConstruct(std::string regist_name);
		virtual Component* create_componenet() = 0;
		void add_component_to(GameObject* game_object);
	};

// 참고1. regist_component는 반드시 기본 생성자를 필요로 함.
#define REGIST_COMPONENT(Class_type)													\
namespace COMPONENTCONSTRUCT {															\
struct ComponentConstruct_##Class_type : public EDITOR::ComponentConstruct				\
{																						\
	ComponentConstruct_##Class_type() : ComponentConstruct(#Class_type) {}				\
	virtual Component* create_componenet() { return new Class_type(); }					\
};																						\
static ComponentConstruct_##Class_type ComponentConstruct_##Class_type##Instance;		\
}

	struct ResourceConstruct
	{
		ResourceConstruct(std::string regist_name);
		void add_resource(std::string resource_name, Engine* engine);
		virtual void create_resource(std::unique_ptr<Resource>& resource) = 0;
	};
#define REGIST_RESOURCE(Class_type)														\
namespace RESOURCECONSTRUCT {															\
struct ResourceConstruct_##Class_type : public EDITOR::ResourceConstruct				\
{																						\
	ResourceConstruct_##Class_type() : ResourceConstruct(#Class_type) {}				\
	virtual void create_resource(std::unique_ptr<Resource>& resource) { resource = std::make_unique<Class_type>(); } \
};																						\
static ResourceConstruct_##Class_type ResourceConstruct_##Class_type##Instance;			\
}

	struct Define
	{
		std::map<std::string, ComponentConstruct*>	component_construct_map;
		std::map<std::string, ResourceConstruct*>	resource_construct_map;

		static Define* get_define();
	};

	void change_last_select(Base* base);
}