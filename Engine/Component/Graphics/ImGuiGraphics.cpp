#include "ImGuiGraphics.h"

#include "../Display/Display.h"
#include "../RenderTarget/RenderToSwapChain.h"

using namespace std;

ImGuiGraphics::ImGuiGraphics(SafePtr<Camera> Camera_ptr, SafePtr<RenderToSwapChain> Render_target, float resolutionWidth, float resolutionHeight) :
	SubGraphics(Camera_ptr, Render_target.cast<RenderTarget>(), resolutionWidth, resolutionHeight)
{
}

ImGuiGraphics::~ImGuiGraphics()
{}

type_index ImGuiGraphics::get_class_type() { return typeid(ImGuiGraphics); }

void ImGuiGraphics::awake()
{
	SubGraphics::awake();

	//Setup ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
																//io.ConfigViewportsNoAutoMerge = true;
																//io.ConfigViewportsNoTaskBarIcon = true;
																//io.ConfigViewportsNoDefaultParent = true;
																//io.ConfigDockingAlwaysTabBar = true;
																//io.ConfigDockingTransparentPayload = true;
																//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
																//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.
	
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();
	//ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGuiStyle* style = &ImGui::GetStyle();
		ImVec4* colors = style->Colors;

		colors[ImGuiCol_Text]                   = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
		colors[ImGuiCol_TextDisabled]           = ImVec4(0.500f, 0.500f, 0.500f, 1.000f);
		colors[ImGuiCol_WindowBg]               = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
		colors[ImGuiCol_ChildBg]                = ImVec4(0.280f, 0.280f, 0.280f, 0.000f);
		colors[ImGuiCol_PopupBg]                = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
		colors[ImGuiCol_Border]                 = ImVec4(0.266f, 0.266f, 0.266f, 1.000f);
		colors[ImGuiCol_BorderShadow]           = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);
		colors[ImGuiCol_FrameBg]                = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
		colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.200f, 0.200f, 0.200f, 1.000f);
		colors[ImGuiCol_FrameBgActive]          = ImVec4(0.280f, 0.280f, 0.280f, 1.000f);
		colors[ImGuiCol_TitleBg]                = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
		colors[ImGuiCol_TitleBgActive]          = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
		colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
		colors[ImGuiCol_MenuBarBg]              = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
		colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
		colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.277f, 0.277f, 0.277f, 1.000f);
		colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.300f, 0.300f, 0.300f, 1.000f);
		colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_CheckMark]              = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
		colors[ImGuiCol_SliderGrab]             = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
		colors[ImGuiCol_SliderGrabActive]       = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_Button]                 = ImVec4(1.000f, 1.000f, 1.000f, 0.000f);
		colors[ImGuiCol_ButtonHovered]          = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
		colors[ImGuiCol_ButtonActive]           = ImVec4(1.000f, 1.000f, 1.000f, 0.391f);
		colors[ImGuiCol_Header]                 = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
		colors[ImGuiCol_HeaderHovered]          = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
		colors[ImGuiCol_HeaderActive]           = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
		colors[ImGuiCol_Separator]              = colors[ImGuiCol_Border];
		colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
		colors[ImGuiCol_SeparatorActive]        = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_ResizeGrip]             = ImVec4(1.000f, 1.000f, 1.000f, 0.250f);
		colors[ImGuiCol_ResizeGripHovered]      = ImVec4(1.000f, 1.000f, 1.000f, 0.670f);
		colors[ImGuiCol_ResizeGripActive]       = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_Tab]                    = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
		colors[ImGuiCol_TabHovered]             = ImVec4(0.352f, 0.352f, 0.352f, 1.000f);
		colors[ImGuiCol_TabActive]              = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
		colors[ImGuiCol_TabUnfocused]           = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
		colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
		colors[ImGuiCol_DockingPreview]         = ImVec4(1.000f, 0.391f, 0.000f, 0.781f);
		colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
		colors[ImGuiCol_PlotLines]              = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
		colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_PlotHistogram]          = ImVec4(0.586f, 0.586f, 0.586f, 1.000f);
		colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_TextSelectedBg]         = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
		colors[ImGuiCol_DragDropTarget]         = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_NavHighlight]           = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
		colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);

		style->ChildRounding = 4.0f;
		style->FrameBorderSize = 1.0f;
		style->FrameRounding = 3.0f;
		style->GrabMinSize = 7.0f;
		style->PopupRounding = 2.0f;
		style->ScrollbarRounding = 12.0f;
		style->ScrollbarSize = 13.0f;
		style->TabBorderSize = 1.0f;
		style->TabRounding = 0.0f;
		style->WindowRounding = 4.0f;
	}

	HWND parent_window = this->Render_target.cast<RenderToSwapChain>()->display->renderWindow->GetHWND();
	ImGui_ImplWin32_Init(parent_window);
	ImGui_ImplDX11_Init(
		this->get_owner()->this_scene->this_engine->Device.Get(),
		this->get_owner()->this_scene->this_engine->Device_context.Get()
	);


	//// 그리기 위해 필요한 최소량.
	//vertex.emplace_back(-0.5, 0.5f, 0.5f, 0.0f, 0.0f);
	//vertex.emplace_back(0.5f, 0.5f, 0.5f, 0.0f, 0.0f);
	//vertex.emplace_back(0.5f, -0.5f, -0.5f, 0.0f, 0.0f);
	//vertex.emplace_back(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f);
	//SV_vertex.initialize(this->Device, vertex);

	//index.emplace_back(0);
	//index.emplace_back(1);
	//index.emplace_back(2);
	//index.emplace_back(3);
	//index.emplace_back(2);
	//index.emplace_back(1);
	//IB_default.initialize(this->Device, index);

	//vector<D3D11_INPUT_ELEMENT_DESC> input_ele;
	//BTYPE::VB_SV_vertex::T_get_input_layout(input_ele);
	//VS_shader.initialize(this->Device, L"DefaultSV_vs_shader", input_ele);
	//this->Device->CreateInputLayout(input_ele.data(), input_ele.size(), this->VS_shader.GetBuffer()->GetBufferPointer(), this->VS_shader.GetBuffer()->GetBufferSize(), input_layout.GetAddressOf());

	//PS_shader.initialize(this->Device, L"DefaultSV_ps_shader");

	//CD3D11_RASTERIZER_DESC rzDesc(D3D11_DEFAULT);
	//rzDesc.CullMode = D3D11_CULL_NONE;
	//this->Device->CreateRasterizerState(&rzDesc, rsstate.GetAddressOf());

	////Create Blend State
	//D3D11_BLEND_DESC blendDesc{};
	//D3D11_RENDER_TARGET_BLEND_DESC rtbd{};
	//rtbd.BlendEnable = true;
	//rtbd.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	//rtbd.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
	//rtbd.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	//rtbd.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
	//rtbd.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
	//rtbd.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	//rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;
	//blendDesc.RenderTarget[0] = rtbd;
	//this->Device->CreateBlendState(&blendDesc, blendState.GetAddressOf());

	//CD3D11_DEPTH_STENCIL_DESC dsDesc(D3D11_DEFAULT);
	////dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
	//this->Device->CreateDepthStencilState(&dsDesc, dsState.GetAddressOf());
}

void ImGuiGraphics::sleep()
{
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiGraphics::update()
{
	// 렌더링 파이프라인을 움직이고.
	Render_target.get()->set_render_target(this);
	Render_target.get()->ClearRenderTarget(this);

	// Window message를 먼저 픽해주고
	MSG msg;
	while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);

		//this->Render_target.cast<RenderToSwapChain>()->display->renderWindow->record(msg.hwnd, msg.message, msg.wParam, msg.lParam);
	}
	
	// ImGui프레임을 만든후
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// ImGui 드로우 데이터를 만든다.
	for (auto& ImGui_draw : this->ImGui_draw_list)
	{
		if (ImGui_draw.is_vaild()) ImGui_draw->draw_detail_view();
	}
	// ImGui을 렌더한후.
	ImGui::Render();

	// 렌더를 그린다.
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	//{
		//////// 테스트 하는 공간.
		//MESH::HaveSV SV_have;
		//SV_have.Vertex_vector.emplace_back(-1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
		//SV_have.Vertex_vector.emplace_back(1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
		//SV_have.Vertex_vector.emplace_back(1.0f, -1.0f, 0.0f, 0.0f, 0.0f);
		//SV_have.Vertex_vector.emplace_back(-1.0f, -1.0f, 0.0f, 0.0f, 0.0f);
		//SV_have.Index_vector.emplace_back(0);
		//SV_have.Index_vector.emplace_back(1);
		//SV_have.Index_vector.emplace_back(2);
		//SV_have.Index_vector.emplace_back(3);
		//SV_have.Index_vector.emplace_back(2);
		//SV_have.Index_vector.emplace_back(1);
		//SHADER::SVShader SV_shader(L"DefaultSV_vs_shader", L"DefaultSV_ps_shader");
		//SV_shader.rasterizer_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		//SV_have.load_resource(this->owner->this_scene->this_engine.get());
		//SV_shader.load_resource(this->owner->this_scene->this_engine.get());
		//UINT Offset = 0;
		//SV_have.set_mesh(this->Device_context, Offset);
		//SV_shader.set_shader(this->Device_context);
		//SV_have.draw(this->Device_context);
	//  //
	//  //
	//	// 그리기 위해서 필요한 최소한. 렌더타겟이 설정되어 있다는 전제하에.
	//	//UINT offset = 0;
	//	//
	//	//this->Device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//	//this->Device_context->RSSetState(rsstate.Get());
	//	//this->Device_context->OMSetBlendState(blendState.Get(), nullptr, 0xffffffff);
	//	//this->Device_context->OMSetDepthStencilState(dsState.Get(), 0);
	//	//this->Device_context->IASetInputLayout(input_layout.Get());
	//	//this->Device_context->VSSetShader(VS_shader.GetShader(), 0, 0);
	//	//this->Device_context->PSSetShader(PS_shader.GetShader(), 0, 0);
	//	//this->Device_context->IASetVertexBuffers(0, 1, SV_vertex.GetAddressOf(), SV_vertex.StridePtr(), &offset);
	//	//this->Device_context->IASetIndexBuffer(IB_default.Get(), DXGI_FORMAT_R32_UINT, 0);
	//	////this->Device_context->DrawAuto();
	//	//this->Device_context->DrawIndexed(index.size(), 0, 0);
	//}
	//only_draw();

	// 
	Render_target.get()->Present(this);
}
