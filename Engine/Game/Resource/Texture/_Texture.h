#pragma once

#include "../Resource.h"
#include "../../Graphics/Color.h"

namespace TEXTURE { class TextureData; }
class Engine;

namespace TEXTURE
{
	class Texture : public Resource
	{
	public:
		Texture() {}

		std::string source_path;
		CD3D11_SAMPLER_DESC sampler_desc;

		Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_state;
		Microsoft::WRL::ComPtr<ID3D11Resource> texture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView;
	public:
		Texture(std::string source_path);
		virtual ~Texture() {}
		virtual std::type_index get_class_type() override { return typeid(Texture); }

		ID3D11SamplerState* GetSamplerState();
		ID3D11SamplerState** GetSamplerStateAddress();
		ID3D11ShaderResourceView* GetTextureResourceView();
		ID3D11ShaderResourceView** GetTextureResourceViewAddress();

		virtual void load_resource(Engine* engine) override;
		virtual void release_resource(Engine* engine) override;

		// 컴포너트가 뭐였더라,,,
		// dds의 경우 컴포넌트를 이용 한 걸로 아는데.
		std::unique_ptr<TextureData> copied_buffer(ID3D11Device* device, ID3D11DeviceContext* device_context, int Component);
		// component의 경우 dds texture에서 dds texture의 중첩? 정도를 나타내는데 사용됨.
		// 추후에 없앨 수 있을 걸로 생각됨.
		void make_texture_data(
			std::unique_ptr<TextureData>& texture_data, 
			int component = 1
		);
		
		virtual void draw_detail_view() override;
	};
}