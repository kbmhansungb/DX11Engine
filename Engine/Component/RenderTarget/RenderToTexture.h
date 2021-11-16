#pragma once

#include "_RenderTarget.h"
#include "../../Game/Pipeline/SpritePipe/SpritePipe.h"


#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: �����丵 �ؾ���")
// unique_ptr�� �����Ǵ� �ؽ��� ���� ��� ���������� �� �� ������?

/// <summary>
/// ��ü������ �ؽ��ķ� ����. �ܺο� ���� ��� �ϳ��� �ؽ��Ŀ� �����ؽ��İ� ����� �� ����.
/// ��ɰ� �����ڰ� �������� ���� �� ������. ����� �Ǽ� �ϱ� ����.
/// 21.11.16
/// </summary>
class RenderToTexture : public RenderTarget
{
public:
	UINT width;
	UINT height;

	std::unique_ptr<TEXTURE::RenderTargetTexture> render_target_texture_0;
	std::unique_ptr<TEXTURE::RenderTargetTexture> render_target_texture_1;
	std::unique_ptr<TEXTURE::RenderTargetTexture> render_target_texture_2;
	std::unique_ptr<TEXTURE::RenderTargetTexture> render_target_texture_3;
	std::unique_ptr<TEXTURE::RenderTargetTexture> render_target_texture_4;
	std::unique_ptr<TEXTURE::RenderTargetTexture> render_target_texture_5;
	std::unique_ptr<TEXTURE::RenderTargetTexture> render_target_texture_6;
	std::unique_ptr<TEXTURE::RenderTargetTexture> render_target_texture_7;

	/// <summary>
	/// 0 : base color
	/// 1 : normal
	/// 2 : result color
	/// 3 : light weight
	/// 4 : world position
	/// </summary>
	std::vector<SafePtr<TEXTURE::RenderTargetTexture>> render_target_textures;
	std::vector<ID3D11RenderTargetView*> render_target_views; // SV_target

	CD3D11_TEXTURE2D_DESC depth_stencil_desc;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depth_stencil_texture;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depth_stencil_view;

	XMFLOAT4 Clear_color{0.0f, 0.0f, 0.0f, 1.0f};
	
	RenderToTexture(UINT resource_width, UINT resource_height);
	
	virtual void awake() override;

	virtual void set_render_target(SubGraphics* sub_graphics) override;
	virtual void ClearRenderTarget(SubGraphics* sub_graphics) override;

	void resize_render_texture(UINT width, UINT height);
	void set_render_texture(SubGraphics* sub_graphics);

	virtual void draw_detail_view() override;
};

class RenderToDeferred;
class DeferredProcess : public SHADER::MeshShader
{
public:
	virtual void deferred_process(RenderToDeferred* rtd, SubGraphics* sub_graphics);
};

class RenderToDeferred : public RenderToTexture
{
public:
	RenderToDeferred() : RenderToDeferred(300, 300) {}
	RenderToDeferred(int resource_width, int resource_height);
	
	std::list<SafePtr<DeferredProcess>> deferred_processes;

	virtual void Present(SubGraphics* sub_graphics) override;
	virtual void draw_detail_view() override;

	void copy_render_texture(
		SubGraphics* sub_graphics,
		Microsoft::WRL::ComPtr<ID3D11Texture2D>& texture_2d,
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shader_resource_view,
		Microsoft::WRL::ComPtr<ID3D11SamplerState>& sampler_state,
		TEXTURE::RenderTargetTexture* render_target_texture
	);
};
REGIST_COMPONENT(RenderToDeferred);