#include "Resource.h"

#include "../Engine.h"

Resource::Resource() {}

Resource::~Resource() {}

std::type_index Resource::get_class_type() { return typeid(Resource); }

Name Resource::get_name()
{
	return Registed_name;
}

void Resource::draw_detail_view()
{
	Base::draw_detail_view();

	if (ImGui::CollapsingHeader("Resource"))
	{
		// rename button
		{
			static std::string name = "";
			static Resource* last_resource = nullptr;

			if (last_resource != this)
			{
				last_resource = this;
				name = this->get_name().get_string();
			}

			ImGui::Text_field(name, "##rename", 100);
			if (ImGui::RightAngleButton("Re name"))
			{
				auto shared = EDITOR::Shared::get_shared();
				rename(shared->engine, name);
			}
		}
		ImGui::NewLine();
		if (ImGui::RightAngleButton("Load resource"))
		{
			this->load_resource(EDITOR::Shared::get_shared()->engine);
		}
		ImGui::NewLine();
		if (ImGui::RightAngleButton("Release resource"))
		{
			this->release_resource(EDITOR::Shared::get_shared()->engine);
		}
	}
}

void Resource::rename(Engine* engine, std::string new_name)
{
	Name old_name = this->Registed_name;
	this->Registed_name = Name(&engine->name_container, new_name);
	auto& old_unique = engine->resources.Resource_map[old_name];
	auto& new_unique = engine->resources.Resource_map[this->Registed_name];
	new_unique.reset(old_unique.release());
	engine->resources.Resource_map.erase(old_name);
}
