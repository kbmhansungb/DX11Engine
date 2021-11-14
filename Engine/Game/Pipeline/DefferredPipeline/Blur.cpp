#include "Blur.h"

Blur::Blur()
{
	this->raw_ms.pixel_shader_path = L"process_Blur_ps";

	auto ec = EngineContext::get_instance();
	blur.initialize(ec->Device.Get());
	blur.apply_changes(ec->Device_context.Get());
}

void Blur::draw_detail_view()
{
	DeferredProcess::draw_detail_view();

	if (ImGui::CollapsingHeader("Blur"))
	{
		ImGui::float_field(&blur.data.min, "Min");
		ImGui::float_field(&blur.data.middle, "Middle");
		ImGui::float_field(&blur.data.max, "Max");
		int i = blur.data.sample;
		ImGui::INT_field(&i, "Sample");
		if (i < 0) i = 0;
		blur.data.sample = i;

		auto ec = EngineContext::get_instance();
		blur.apply_changes(ec->Device_context.Get());
	}
}

void Blur::deferred_process(RenderToDeferred* rtd, SubGraphics* sub_graphics)
{
	blur.set_constant_buffer(sub_graphics->Device_context);

	// world position을 copy해서 넘겨야함.
	Microsoft::WRL::ComPtr<ID3D11Texture2D> bc_t2d, lw_t2d;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> bc_srv, lw_srv;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> bc_ss, lw_ss;
	rtd->copy_render_texture(sub_graphics, bc_t2d, bc_srv, bc_ss, rtd->render_target_texture_2.get());
	rtd->copy_render_texture(sub_graphics, lw_t2d, lw_srv, lw_ss, rtd->render_target_texture_5.get());
	ID3D11ShaderResourceView* srv[] = {
		bc_srv.Get(),
		lw_srv.Get()
	};
	ID3D11SamplerState* ss[] = {
		bc_ss.Get(),
		lw_ss.Get()
	};
	sub_graphics->Device_context->PSSetShaderResources(
		0, 2, srv
	);
	sub_graphics->Device_context->PSSetSamplers(
		0, 2, ss
	);
	DeferredProcess::deferred_process(rtd, sub_graphics);
}
