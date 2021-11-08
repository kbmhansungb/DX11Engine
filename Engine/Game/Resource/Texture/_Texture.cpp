#include "_Texture.h"
#include "_TextureData.h"
#include "../../Engine.h"
#include "../../../Template/Editor_ImageView/Editor_ImageView.h"

using namespace std;

TEXTURE::Texture::Texture(string source_path) :
	source_path(source_path),
	sampler_desc(D3D11_DEFAULT)
{
	// sampler_desc
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
}

ID3D11SamplerState* TEXTURE::Texture::GetSamplerState()
{
	return this->sampler_state.Get();
}

ID3D11SamplerState** TEXTURE::Texture::GetSamplerStateAddress()
{
	return this->sampler_state.GetAddressOf();
}

ID3D11ShaderResourceView* TEXTURE::Texture::GetTextureResourceView()
{
	return this->textureView.Get();
}

ID3D11ShaderResourceView** TEXTURE::Texture::GetTextureResourceViewAddress()
{
	return this->textureView.GetAddressOf();
}

void TEXTURE::Texture::load_resource(Engine* engine)
{
	HRESULT hr;

	hr = engine->Device->CreateSamplerState(
		&sampler_desc,
		sampler_state.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create sampler state.");
}

void TEXTURE::Texture::release_resource(Engine* engine)
{
	sampler_state.Reset();
	texture.Reset();
	textureView.Reset();
}


// ������ �� ������ �ִ��� �𸣰ڳ�.

unique_ptr<TEXTURE::TextureData> TEXTURE::Texture::copied_buffer(ID3D11Device* device, ID3D11DeviceContext* device_context, int Component)
{
	if (StringHelper::GetFileExtension(source_path) == "dds")
	{
		// dds ������ ���.. 
		// �� �̷��� �ؾ��ϴ��� ��Ծ���.
		ID3D11Texture2D* texture2D = static_cast<ID3D11Texture2D*>(texture.Get());
		Microsoft::WRL::ComPtr<ID3D11Texture2D> copied;

		// ������¡ �ؽ��� ����
		D3D11_TEXTURE2D_DESC describe; texture2D->GetDesc(&describe);
		describe.Usage = D3D11_USAGE_STAGING; // 
		describe.CPUAccessFlags = D3D11_CPU_ACCESS_READ; // need staging flag for read
		describe.BindFlags = 0; // no bind flags allowed for staging
		HRESULT hr = device->CreateTexture2D(&describe, 0, copied.GetAddressOf());
		device_context->CopyResource(copied.Get(), texture2D);

		D3D11_MAPPED_SUBRESOURCE resource{};
		device_context->Map(copied.Get(), 0, D3D11_MAP_READ, 0, &resource);
		std::unique_ptr<TextureData> texture_data = make_unique<TextureData>(
			describe.Width,
			describe.Height,
			Component);

		BYTE* sptr = reinterpret_cast<BYTE*>(resource.pData);
		{
			memcpy_s(
				texture_data.get()->Byte_array.data(), describe.Width * describe.Height * Component * sizeof(float),
				sptr, describe.Width * describe.Height * Component * sizeof(Color));
		}
		device_context->Unmap(copied.Get(), 0);
		return texture_data;
	}
	else
	{
		// dds ������ �ƴ� ���
		ID3D11Texture2D* texture2D = static_cast<ID3D11Texture2D*>(texture.Get());
		Microsoft::WRL::ComPtr<ID3D11Texture2D> copied;

		// ������¡ �ؽ��� ����
		D3D11_TEXTURE2D_DESC describe; texture2D->GetDesc(&describe);
		describe.Usage = D3D11_USAGE_STAGING; // 
		describe.CPUAccessFlags = D3D11_CPU_ACCESS_READ; // need staging flag for read
		describe.BindFlags = 0; // no bind flags allowed for staging
		HRESULT hr = device->CreateTexture2D(&describe, 0, copied.GetAddressOf());
		device_context->CopyResource(copied.Get(), texture2D);

		D3D11_MAPPED_SUBRESOURCE resource{};
		UINT subresource = D3D11CalcSubresource(0, 0, 0);
		device_context->Map(copied.Get(), subresource, D3D11_MAP_READ, 0, &resource);
		std::unique_ptr<TextureData> texture_data = make_unique<TextureData>(
			describe.Width,
			describe.Height);
		BYTE* sptr = reinterpret_cast<BYTE*>(resource.pData);
		{
			memcpy_s(
				texture_data.get()->Byte_array.data(), resource.RowPitch * describe.Height,
				sptr, resource.RowPitch * describe.Height);
		}
		device_context->Unmap(copied.Get(), subresource);
		return texture_data;
	}
}

void TEXTURE::Texture::make_texture_data(
	std::unique_ptr<TextureData>& texture_data,
	int component
)
{
	// �ؽ��İ� ���� ��� ������.
	if (texture_data.get() != nullptr) texture_data.reset();
	auto& context = *EngineContext::get_instance();

	// ������¡ �ؽ��ĸ� ���� ����
	ID3D11Texture2D* texture2D = static_cast<ID3D11Texture2D*>(texture.Get());
	Microsoft::WRL::ComPtr<ID3D11Texture2D> copied;

	// ������¡ �ؽ��� ����
	D3D11_TEXTURE2D_DESC describe; texture2D->GetDesc(&describe);
	describe.Usage = D3D11_USAGE_STAGING; // 
	describe.CPUAccessFlags = D3D11_CPU_ACCESS_READ; // need staging flag for read
	describe.BindFlags = 0; // no bind flags allowed for staging
	HRESULT hr = context.Device->CreateTexture2D(&describe, 0, copied.GetAddressOf());
	context.Device_context->CopyResource(copied.Get(), texture2D);

	texture_data = make_unique<TextureData>(
		describe.Width,
		describe.Height,
		component);

	D3D11_MAPPED_SUBRESOURCE resource{};
	if (StringHelper::GetFileExtension(source_path) == "dds")
	{
		// dds texture�� ���

		// ���� �Ƹ��� �ϳ�.. �ּ��� �ް� ������.

		UINT subresource = D3D11CalcSubresource(0, 0, 0);
		context.Device_context->Map(copied.Get(), 0, D3D11_MAP_READ, 0, &resource);
		BYTE* sptr = reinterpret_cast<BYTE*>(resource.pData);
		{
			memcpy_s(
				texture_data.get()->Byte_array.data(), describe.Width * describe.Height * component * sizeof(float),
				sptr, describe.Width * describe.Height * component * sizeof(Color));
		}
		context.Device_context->Unmap(copied.Get(), subresource);
	} 
	else
	{
		// dds�� �ƴ� ���
		UINT subresource = D3D11CalcSubresource(0, 0, 0);
		context.Device_context->Map(copied.Get(), subresource, D3D11_MAP_READ, 0, &resource);
		BYTE* sptr = reinterpret_cast<BYTE*>(resource.pData);
		{
			memcpy_s(
				texture_data.get()->Byte_array.data(), resource.RowPitch * describe.Height,
				sptr, resource.RowPitch * describe.Height);
		}
		context.Device_context->Unmap(copied.Get(), subresource);
	}
}

void TEXTURE::Texture::draw_detail_view()
{
	Resource::draw_detail_view();

	if (ImGui::CollapsingHeader("Texture"))
	{
		ImGui::NewLine();
		std::string free_view_button
			= "Free view##"
			+ StringHelper::ptr_to_string(this);
		if (ImGui::RightAngleButton(free_view_button.c_str()))
		{
		}

		std::string sampler_state_text
			= "Sampler state : ";
		sampler_state_text
			+= (this->sampler_state.Get() != nullptr ? "Loaded" : "Unloaded");
		ImGui::Text(sampler_state_text.c_str());
	}
}
