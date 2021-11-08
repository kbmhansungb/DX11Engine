#pragma once

#include "_load_resource.h"
#include "ToolMain_ImGui.h"

namespace ModelTool
{
	class ModelToolImGui : public ViaRenderer
	{
	public:
		SafePtr<ModelRenderer> Model_renderer;

		virtual void draw(SubGraphics* Sub_graphics) override
		{
			auto Data = ToolData::get_map_tool_data();
			ImGuiWindowFlags Window_flags = 0;

			if (Data->Tool_main_ImGui->State
				== ToolMainImGui::TOOLMAINGUISTATE::TOOLMAINGUISTATE_MODEL_TOOL)
			{
				Window_flags = 0;
				Window_flags |= ImGuiWindowFlags_::ImGuiWindowFlags_HorizontalScrollbar;
				ImGui::Begin("Model tool", 0, Window_flags);
				{
					if (Model_renderer.is_vaild())
						ImGui::Text(Model_renderer->Render_target_model->get_name().get_string().c_str());
					else
						ImGui::Text("Is not valid.");
				}
				ImGui::End();
			}
		}
		virtual void get_hit_result(RayTracingLib::Ray* ray, RayTracingLib::HitResult* hit_result, float ray_max_distance) override {}
	};
}