#pragma once

#include "_Buffer.h"

template<typename Type>
class AccessibleBuffer
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
private:
	AccessibleBuffer(const AccessibleBuffer<Type>& rhs) = delete;
	AccessibleBuffer(AccessibleBuffer<Type>&& rhs) = delete;
	void operator= (const AccessibleBuffer<Type>& rhs) = delete;
public:
	AccessibleBuffer() {}
	ID3D11Buffer* Get() const
	{
		return buffer.Get();
	}
	ID3D11Buffer* const* GetAddressOf() const
	{
		return buffer.GetAddressOf();
	}
	void create_buffer(ID3D11Device* Device, ID3D11DeviceContext* Device_context, ID3D11Buffer* Copied_buffer)
	{
		this->buffer.Reset();

		D3D11_BUFFER_DESC target_buffer_describe{};
		Copied_buffer->GetDesc(&target_buffer_describe);

		
		//initialize Constant Buffer
		D3D11_BUFFER_DESC buffer_describe{};
		buffer_describe.ByteWidth = target_buffer_describe.ByteWidth;

		//buffer_describe.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT,

		buffer_describe.Usage = D3D11_USAGE_DEFAULT;
		buffer_describe.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		buffer_describe.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
		buffer_describe.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

		//buffer_describe.Usage = D3D11_USAGE_DYNAMIC;
		//buffer_describe.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		//buffer_describe.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		//buffer_describe.MiscFlags = 0;

		//buffer_describe.StructureByteStride = 0;// target_buffer_describe.StructureByteStride;
		buffer_describe.StructureByteStride = sizeof(Type);// target_buffer_describe.StructureByteStride;
		// CPU_ACCESS_READ가 붙을때 StructureByteStride는 필수이다.

		HRESULT hr = Device->CreateBuffer(&buffer_describe, NULL, this->buffer.GetAddressOf());
		//Device_context->CopyResource(buffer.Get(), Copied_buffer);
		Device_context->CopyResource(Copied_buffer, buffer.Get());
	}
	void release()
	{
		this->buffer.Reset();
	}
private:
		template <typename Type>
		struct AutoMapUnmap
		{
		public:
			ID3D11Buffer* buffer;
			ID3D11DeviceContext* device_context;

			D3D11_MAPPED_SUBRESOURCE mapRes;
			Type* data;
			AutoMapUnmap(ID3D11Buffer* buffer, ID3D11DeviceContext* device_context) :
				buffer(buffer), device_context(device_context)
			{
				device_context->Map(buffer, 0, D3D11_MAP_READ_WRITE, 0, &mapRes);
				data = (Type*)mapRes.pData;
			}
			~AutoMapUnmap()
			{
				device_context->Unmap(buffer, 0);
				data = nullptr; // CPU에서 접근하지 못하게 막아두어야 한다. 자동으로 삭제되기는 하지만 꺼두자.
			}
		private:
			//AutoMapUnmap(const AutoMapUnmap< Type>& rhs) = delete;
		};
public:
	AutoMapUnmap<Type> track_changes(ID3D11DeviceContext* deviceContext)
	{
		return AutoMapUnmap<Type>(this->buffer.Get(), deviceContext);
	}
};