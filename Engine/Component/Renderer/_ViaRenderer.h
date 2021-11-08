#pragma once

#include "../_Include.h"

class ViaRenderer : public Component, public RenderScope
{
public:
	// RenderScope을(를) 통해 상속됨
	virtual void set_visibility(bool new_visiblity) override
	{
		RenderScope::set_visibility(new_visiblity, this->get_owner()->this_scene.get());
	}
	virtual void draw(SubGraphics* sub_graphics) override = 0;
	virtual void draw_mesh_only(SubGraphics* sub_graphics) override = 0;
	virtual void draw_detail_view()
	{
		Component::draw_detail_view();

		if (ImGui::CollapsingHeader("Via renderer"))
		{
			static bool temp_visibility;
			temp_visibility = this->VIsiblity;
			if (ImGui::Checkbox("Set visibility", &temp_visibility))
			{
				set_visibility(temp_visibility);
			}
		}
	}

};