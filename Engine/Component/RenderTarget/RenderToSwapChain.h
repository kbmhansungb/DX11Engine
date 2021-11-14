#pragma once

#include "_RenderTarget.h"
class Display;

class RenderToSwapChain final : public RenderTarget
{
public:
	SafePtr<Display> display;
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	CD3D11_TEXTURE2D_DESC depth_stencil_texture_desc;

	XMFLOAT4 Clear_color{ 0.0f, 0.0f, 0.0f, 1.0f };
private:
	// device and device context
	Microsoft::WRL::ComPtr<IDXGISwapChain> _swapchain;
public:
	// buffer
	Microsoft::WRL::ComPtr<ID3D11Texture2D> render_texture;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> render_target_view;
	// depth stencil
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depth_stencil_texture;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depth_stencil_view;
public:
	RenderToSwapChain();
	virtual ~RenderToSwapChain();
	RenderToSwapChain(SafePtr<Display> display);
	//virtual bool awake_condition();
	virtual void awake() override;
	virtual void sleep() override;
	virtual void set_render_target(SubGraphics* sub_graphics) override;
	virtual void ClearRenderTarget(SubGraphics* sub_graphics) override;
	// 스왑체인후 리무브 디바이스가 호출된다. 어째서 인가.
	virtual void Present(SubGraphics* sub_graphics) override;
	virtual void draw_detail_view() override;
private:
	void response_resize_display(UINT width, UINT height);

	void clear_swap_chain();
	void create_swap_chain();
};
REGIST_COMPONENT(RenderToSwapChain);