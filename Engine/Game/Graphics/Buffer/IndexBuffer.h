#pragma once

#include "_Buffer.h"

template <typename Index>
class IndexBuffer
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	UINT indexCount = 0;
public:
	IndexBuffer() 
	{
	}
	virtual ~IndexBuffer()
	{
	}
	ID3D11Buffer* Get() const
	{
		return buffer.Get();
	}
	ID3D11Buffer* const* GetAdressOf() const
	{
		return buffer.GetAddressOf();
	}
	UINT IndexCount() const
	{
		return indexCount;
	}
	DXGI_FORMAT get_index_buffer_type()
	{
		return Index::T_get_index_buffer_type();
	}
	void initialize(ID3D11Device* device, Index* data, UINT numIndices)
	{
		free_buffer();

		if (numIndices == 0) return;

		this->indexCount = numIndices;
		//Index Buffer
		D3D11_BUFFER_DESC ibDesc{};
		ibDesc.ByteWidth = sizeof(Index) * numIndices;
		ibDesc.Usage = D3D11_USAGE_DEFAULT;
		ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibDesc.CPUAccessFlags = 0;
		ibDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA ibData{};
		ibData.pSysMem = data;

		HRESULT hr = device->CreateBuffer(&ibDesc, &ibData, this->buffer.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create buffer.");
	}
	void free_buffer()
	{
		buffer.Reset();
	}
};