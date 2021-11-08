#pragma once

#include "_SubGraphics.h"
class RenderToSwapChain;
class ImGuiGraphics : public SubGraphics
{
public:
	std::list<SafePtr<Base>> ImGui_draw_list;

	// test

	//vector<BTYPE::VB_SV_vertex> vertex;
	//vector<BTYPE::IB_Default> index;
	//SHADER::VertexShader VS_shader;
	//SHADER::PixelShader PS_shader;
	//VertexBuffer<BTYPE::VB_SV_vertex> SV_vertex;
	//IndexBuffer<BTYPE::IB_Default> IB_default;
	//ComPtr<ID3D11InputLayout> input_layout;
	//ComPtr<ID3D11RasterizerState> rsstate;
	//ComPtr<ID3D11BlendState> blendState;
	//ComPtr<ID3D11DepthStencilState> dsState;

	ImGuiGraphics(SafePtr<Camera> Camera_ptr, SafePtr<RenderToSwapChain> Render_target, float resolutionWidth, float resolutionHeight);
	virtual ~ImGuiGraphics();
	virtual std::type_index get_class_type();
	virtual void awake() override;
	virtual void sleep() override;
	virtual void update() override;
};