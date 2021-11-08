#pragma once

#include "_Texture.h"

namespace TEXTURE
{
	class TextureData
	{
	public:
		int Width;
		int Height;
		int Components;

		std::vector<Color> Byte_array;
		TextureData(int Width, int Height, int Components = 1) : Width(Width), Height(Height), Components(Components)
		{
			Byte_array.resize(Width * Height * Components);
		}
		// 참조가 아니라 복사해서 전달.
		Color read_color_with_index(int x_index, int y_index) const
		{
			return Byte_array[x_index + y_index * Width]; // Color는 4BYTE임.
		}
		// 참조가 아니라 복사해서 전달.
		Color read_color_with_coordinate(float u, float v) const
		{
			if (u > 1.0f) u = 1.0f; if (u < 0.0f) u = 0.0f;
			if (v > 1.0f) v = 1.0f; if (v < 0.0f) v = 0.0f;
			// lerp 없으므로 내림해서 그냥 반환한다.

			return read_color_with_index(
				static_cast<int>(u * (Width - 1)), 
				static_cast<int>(v * (Height - 1))
			);
		}

		std::unique_ptr<ID3D11Texture2D> create_dds_texture_from_data(ID3D11Device* device, DXGI_FORMAT format = DXGI_FORMAT_R32_FLOAT, const int components = 1)
		{
			std::unique_ptr<ID3D11Texture2D> return_texture;
			ID3D11Texture2D* texture_2d;

			D3D11_TEXTURE2D_DESC texture_desc;
			// Set up a texture description 
			texture_desc.Width = this->Width;
			texture_desc.Height = this->Height;
			texture_desc.MipLevels = texture_desc.ArraySize = 1;
			texture_desc.Format = format;
			texture_desc.SampleDesc.Count = 1;
			texture_desc.SampleDesc.Quality = 0;
			texture_desc.Usage = D3D11_USAGE_DYNAMIC;
			texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			texture_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			texture_desc.MiscFlags = 0;

			// Initialise buffer parameters
			const int length = components * this->Width * this->Height;
			// Fill buffer with vertex colours
			float* buffer = new float[length * sizeof(float)];
			for (int i = 0; i < length; i++)
				buffer[i] = this->Byte_array[i].dds;

			// Set the texture data using the buffer contents
			D3D11_SUBRESOURCE_DATA texture_data;
			texture_data.pSysMem = (void*)buffer;
			texture_data.SysMemPitch = this->Width * components * sizeof(float);
			// Create the texture using the terrain colour data
			HRESULT hr = device->CreateTexture2D(&texture_desc, &texture_data, &texture_2d);

			return_texture.reset(texture_2d);
			return return_texture;
		}
	};
}