#include "RenderToSwapChain.h"
#include "../Graphics/_SubGraphics.h"
#include "../Display/Display.h"

using namespace std;

RenderToSwapChain::RenderToSwapChain() :
	display(SafePtr<Display>()),
	depth_stencil_texture_desc(DXGI_FORMAT_D24_UNORM_S8_UINT, 0, 0)
{
	// swapChainDesc
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// depthStencilBufferDesc
	depth_stencil_texture_desc.MipLevels = 1;
	depth_stencil_texture_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
}

RenderToSwapChain::~RenderToSwapChain()
{
}

RenderToSwapChain::RenderToSwapChain(SafePtr<Display> display) :
	display(display),
	depth_stencil_texture_desc(DXGI_FORMAT_D24_UNORM_S8_UINT, 0, 0)
{
	// swapChainDesc
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// depthStencilBufferDesc
	depth_stencil_texture_desc.MipLevels = 1;
	depth_stencil_texture_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
}

//bool RenderToSwapChain::awake_condition()
//{
//	return display.is_vaild();
//}

void RenderToSwapChain::awake()
{
	// display가 유효하지 않으면 켜지지 않음.
	if (display.is_vaild() == false)
	{
		assert("Display is not vaild.");
	}
	display->delegate_resize_window
		.AddInvoker(
			this, 
			&RenderToSwapChain::response_resize_display
		);

	create_swap_chain();
}

void RenderToSwapChain::sleep()
{
	clear_swap_chain();
}

void RenderToSwapChain::set_render_target(SubGraphics* sub_graphics)
{
	sub_graphics->Device_context->OMSetRenderTargets(1, render_target_view.GetAddressOf(), depth_stencil_view.Get());
}

void RenderToSwapChain::ClearRenderTarget(SubGraphics* sub_graphics)
{
	sub_graphics->Device_context->ClearRenderTargetView(render_target_view.Get(), &Clear_color.x);
	sub_graphics->Device_context->ClearDepthStencilView(depth_stencil_view.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

// 스왑체인후 리무브 디바이스가 호출된다. 어째서 인가.

void RenderToSwapChain::Present(SubGraphics* sub_graphics)
{
	this->_swapchain->Present(0, 0);
}

void RenderToSwapChain::draw_detail_view()
{
	Component::draw_detail_view();

	if (ImGui::CollapsingHeader("Render to swap chain"))
	{
		ImGui::base_field(display);
	}
}

void RenderToSwapChain::response_resize_display(UINT width, UINT height)
{
	// condition
	if (width == 0 || height == 0) return;

	clear_swap_chain();
	create_swap_chain();
}

void RenderToSwapChain::clear_swap_chain()
{
	_swapchain.Reset();
	render_texture.Reset();
	render_target_view.Reset();
	depth_stencil_texture.Reset();
	depth_stencil_view.Reset();
}

void RenderToSwapChain::create_swap_chain()
{
	swapChainDesc.BufferDesc.Width = display->windowWidth;
	swapChainDesc.BufferDesc.Height = display->windowHeight;
	swapChainDesc.OutputWindow = display.get()->renderWindow->GetHWND();

	depth_stencil_texture_desc.Width = display->windowWidth;
	depth_stencil_texture_desc.Height = display->windowHeight;

	// 렌더 영역
	HRESULT hr;

	// Engine의 영역
	ID3D11Device* Device = owner->this_scene->this_engine->Device.Get(); //이 디바이스는 최초의 디바이스,
																				 // Factory로 스왑체인 생성
	IDXGIFactory* dxgiFactory = nullptr;
	{
		// Facctory 생성 코드
		IDXGIDevice* dxgiDevice = nullptr;
		hr = Device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
		if (SUCCEEDED(hr))
		{
			IDXGIAdapter* adapter = nullptr;
			hr = dxgiDevice->GetAdapter(&adapter);
			if (SUCCEEDED(hr))
			{
				hr = adapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiFactory));
				adapter->Release();
			}
			dxgiDevice->Release();
		}
	}
	dxgiFactory->CreateSwapChain(Device, &swapChainDesc, _swapchain.GetAddressOf());

	hr = this->_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(render_texture.GetAddressOf()));
	COM_ERROR_IF_FAILED(hr, "Failed to get back buffer.");

	// buffer
	hr = Device->CreateRenderTargetView(render_texture.Get(), 0, render_target_view.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create render target view.");
	//DepthStencil
	hr = Device->CreateTexture2D(&depth_stencil_texture_desc, 0, depth_stencil_texture.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Faield to create texture 2D.");
	hr = Device->CreateDepthStencilView(depth_stencil_texture.Get(), 0, depth_stencil_view.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Faield to create depth stencil view.");
}
