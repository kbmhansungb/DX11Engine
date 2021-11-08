#pragma once

#include "_Texture.h"

namespace TEXTURE
{
	// render target texture는 R32G32B32A32만 받는다.
	class RenderTargetTexture : public Texture
	{
	public:
		CD3D11_TEXTURE2D_DESC texture_desc;
		CD3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc;
		CD3D11_RENDER_TARGET_VIEW_DESC render_target_view_desc;

		//ComPtr<ID3D11Texture2D> texture;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture_2d;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> render_target_view;

		RenderTargetTexture(UINT resource_width, UINT resource_height);
		void resize_texture(float width, float height, Engine* engine);
		virtual void release_resource(Engine* engine) override;
		virtual void load_resource(Engine* engine) override;

		virtual void draw_detail_view() override;
	};
}