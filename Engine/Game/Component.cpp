#include "Component.h"

Component::Component()
{
}

Component::~Component()
{
}

void Component::draw_detail_view()
{
	Base::draw_detail_view();

	if (ImGui::CollapsingHeader("Componenet"))
	{
		bool temp_activity = this->active;
		if (ImGui::Checkbox("Put activity", &temp_activity))
		{
			this->active = temp_activity;
		}
	}
}
