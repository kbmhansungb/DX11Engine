#include "LightProcess.h"

LightProcess::LightProcess()
{
	this->raw_ms.pixel_shader_path = L"process_Light_ps";

	auto ec = EngineContext::get_instance();
	light_process.initialize(ec->Device.Get());
	light_process.apply_changes(ec->Device_context.Get());
}
void LightProcess::draw_detail_view()
{
	DeferredProcess::draw_detail_view();

	if (ImGui::CollapsingHeader("Light process"))
	{
		int i = light_process.data.sample_count;
		ImGui::INT_field(&i, "Sample count");
		if (i < 0) i = 0;
		light_process.data.sample_count = i;

		auto ec = EngineContext::get_instance();
		light_process.apply_changes(ec->Device_context.Get());
	}
}

void LightProcess::deferred_process(RenderToDeferred* rtd, SubGraphics* sub_graphics)
{
	// world position을 copy해서 넘겨야함.
	Microsoft::WRL::ComPtr<ID3D11Texture2D> bc_t2d, lw_t2d, wp_t2d;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> bc_srv, lw_srv, wp_srv;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> bc_ss, lw_ss, wp_ss;
	rtd->copy_render_texture(sub_graphics, bc_t2d, bc_srv, bc_ss, rtd->render_target_texture_0.get());
	rtd->copy_render_texture(sub_graphics, lw_t2d, lw_srv, lw_ss, rtd->render_target_texture_3.get());
	rtd->copy_render_texture(sub_graphics, wp_t2d, wp_srv, wp_ss, rtd->render_target_texture_1.get());
	ID3D11ShaderResourceView* srv[] = {
		bc_srv.Get(),
		lw_srv.Get(),
		wp_srv.Get()
	};
	ID3D11SamplerState* ss[] = {
		bc_ss.Get(),
		lw_ss.Get(),
		wp_ss.Get()
	};
	sub_graphics->Device_context->PSSetShaderResources(
		0, 3, srv
	);
	sub_graphics->Device_context->PSSetSamplers(
		0, 3, ss
	);

	light_process.set_constant_buffer(sub_graphics->Device_context);
	DeferredProcess::deferred_process(rtd, sub_graphics);
}
