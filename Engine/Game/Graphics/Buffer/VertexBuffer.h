#pragma once

#include "_Buffer.h"

/// <summary>
/// 버텍스 버퍼
/// 21.11.16
/// </summary>
/// <typeparam name="Type">BTYPE</typeparam>
template <typename Type>
class VertexBuffer
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	UINT stride = 0;
	UINT vertexCount = 0;

	VertexBuffer(const VertexBuffer& rhs) = delete;
	VertexBuffer(const VertexBuffer&& rhs) = delete;
	void operator=(const VertexBuffer& rhs) {};
public:
	VertexBuffer() {};
	virtual ~VertexBuffer() {};
	ID3D11Buffer* Get() const
	{
		return buffer.Get();
	}

	ID3D11Buffer* const* GetAddressOf() const
	{
		return buffer.GetAddressOf();
	}

	UINT VertexCount() const
	{
		return this->vertexCount;
	}

	const UINT Stride() const
	{
		return this->stride;
	}

	const UINT* StridePtr() const
	{
		return &this->stride;
	}
	void initialize(ID3D11Device* device, Type* data, UINT numVertices)
	{
		free_buffer();

		if (numVertices == 0) return;

		this->vertexCount = numVertices;
		this->stride = sizeof(Type);

		//Vertex Buffer
		D3D11_BUFFER_DESC vbDesc{};
		vbDesc.ByteWidth = this->stride * numVertices;
		vbDesc.Usage = D3D11_USAGE_DEFAULT;
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbDesc.CPUAccessFlags = 0;
		vbDesc.MiscFlags = 0;
		vbDesc.StructureByteStride = sizeof(Type);
		D3D11_SUBRESOURCE_DATA vbData{};
		vbData.pSysMem = data;

		HRESULT hr = device->CreateBuffer(&vbDesc, &vbData, this->buffer.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create buffer");
	}
	void free_buffer()
	{
		buffer.Reset();
	}
};
