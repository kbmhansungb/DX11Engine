#include "Lighting.h"
#include "../../Component/Graphics/_SubGraphics.h"

void LightingContainer::lighting_all(SubGraphics* sub_graphics)
{
	for (auto& lighting : lighting_uor_set)
	{
		lighting->lighting(sub_graphics);
#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: 막쓴 코드")
		sub_graphics->Device_context->DrawIndexed(6, 0, 0);
	}
}

void Lighting::regist_lighting(LightingContainer* lcon)
{
	lcon->lighting_uor_set.insert(this);
}

void Lighting::unregist_lighting(LightingContainer* lcon)
{
	lcon->lighting_uor_set.erase(this);
}

void Lighting::put_affect_world(bool affect_world, LightingContainer* lcon)
{
	if (affect_world)
		regist_lighting(lcon);
	else
		unregist_lighting(lcon);

	this->affect_world = affect_world;
}

void Lighting::draw_lighting_detail_view(LightingContainer* lcon)
{
	std::string check_box_name
		= "Affect_world##"
		+ StringHelper::ptr_to_string(&this->affect_world);
	if (ImGui::Checkbox(check_box_name.c_str(), &this->affect_world))
	{
		put_affect_world(affect_world, lcon);
	}
}
