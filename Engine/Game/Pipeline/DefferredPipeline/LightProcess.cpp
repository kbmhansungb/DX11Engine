#include "LightProcess.h"

LightProcess::LightProcess()
{
	this->raw_ms.pixel_shader_path = L"process_Light_ps";
}
void LightProcess::draw_detail_view()
{
	DeferredProcess::draw_detail_view();

	if (ImGui::CollapsingHeader("Light process"))
	{
	}
}
