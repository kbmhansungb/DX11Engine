#include "_ViaRenderer.h"

void ViaRenderer::set_visibility(bool new_visiblity)
{
	RenderScope::set_visibility(new_visiblity, this->get_owner()->this_scene.get());
}

void ViaRenderer::draw_detail_view()
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
