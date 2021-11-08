#pragma once

#include "_Buffer.h"

template<class Type>
class ConstantBuffer
{
	class Helper
	{
		Helper()
		{
			UINT constant_buffer_slot = Type::T_get_constant_buffer_slot();
			bool use_in_vs = Type::T_use_in_vs();
			bool use_in_ps = Type::T_use_in_ps();
		}
	public:
		inline static int get_constant_buffer_slot()		{ return Type::T_get_constant_buffer_slot(); }
		inline static bool is_use_in_vs()					{ return Type::T_use_in_vs(); }
		inline static bool is_use_in_ps()					{ return Type::T_use_in_ps(); }
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;

	ConstantBuffer(const ConstantBuffer<Type>& rhs);
public:
	ConstantBuffer() {}
	Type data;

	ID3D11Buffer* Get() const
	{
		return buffer.Get();
	}
	ID3D11Buffer* const* GetAddressOf() const
	{
		return buffer.GetAddressOf();
	}
	HRESULT initialize(ID3D11Device* device)
	{
		this->buffer.Reset();

		//initialize Constant Buffer
		D3D11_BUFFER_DESC bfDesc;
		ZeroMemory(&bfDesc, sizeof(D3D11_BUFFER_DESC));
		bfDesc.Usage = D3D11_USAGE_DYNAMIC;
		bfDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bfDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bfDesc.MiscFlags = 0;
		bfDesc.ByteWidth = sizeof(Type) + (16 - sizeof(Type) % 16);
		bfDesc.StructureByteStride = 0;

		return device->CreateBuffer(&bfDesc, 0, this->buffer.GetAddressOf());
	}
	bool apply_changes(ID3D11DeviceContext* device_context)
	{
		try
		{
			D3D11_MAPPED_SUBRESOURCE mapRes;
			HRESULT hr = device_context->Map(this->buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapRes);
			COM_ERROR_IF_FAILED(hr, "Failed to update constant buffer.");
			CopyMemory(mapRes.pData, &this->data, sizeof(Type));
			device_context->Unmap(this->buffer.Get(), 0);
		}
		catch (COMException& exp)
		{
			ErrorLogger::Log(exp);
			return false;
		}
		return true;
	}
	void set_constant_buffer(ID3D11DeviceContext* device_context)
	{
		UINT slot					= Helper::get_constant_buffer_slot();
		auto buffer_address_of		= GetAddressOf();

		if (Helper::is_use_in_vs())
			device_context->VSSetConstantBuffers(slot, 1, buffer_address_of);
		if (Helper::is_use_in_ps())
			device_context->PSSetConstantBuffers(slot, 1, buffer_address_of);
	}
	void set_constant_buffer(ID3D11DeviceContext* device_context, UINT slot)
	{
		auto buffer_address_of = GetAddressOf();

		if (Helper::is_use_in_vs())
			device_context->VSSetConstantBuffers(slot, 1, buffer_address_of);
		if (Helper::is_use_in_ps())
			device_context->PSSetConstantBuffers(slot, 1, buffer_address_of);
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
			device_context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapRes);
			data = (Type*)mapRes.pData;
		}
		~AutoMapUnmap()
		{
			device_context->Unmap(buffer, 0);
			data = nullptr; // CPU에서 접근하지 못하게 막아두어야 한다. 자동으로 삭제되는거 같지만 꺼두자.
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

template <typename Type>
class ConstantArrayBuffer
{
	class Helper
	{
		Helper()
		{
		}
	public:
		inline static int get_constant_buffer_slot() { return Type::T_get_constant_buffer_slot(); }
		inline static bool is_use_in_vs() { return Type::T_use_in_vs(); }
		inline static bool is_use_in_ps() { return Type::T_use_in_ps(); }
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
public:
	std::vector<Type> data;

	ID3D11Buffer* Get() const
	{
		return buffer.Get();
	}
	ID3D11Buffer* const* GetAddressOf() const
	{
		return buffer.GetAddressOf();
	}
	HRESULT initialize(ID3D11Device* device, int array_size)
	{
		data.resize(array_size);
		this->buffer.Reset();

		//initialize Constant Buffer
		D3D11_BUFFER_DESC bfDesc{};
		bfDesc.Usage = D3D11_USAGE_DYNAMIC;
		bfDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bfDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bfDesc.MiscFlags = 0;
		bfDesc.ByteWidth = (sizeof(Type) * array_size) + (16 - (sizeof(Type) * array_size) % 16);
		bfDesc.StructureByteStride = 0;

		return device->CreateBuffer(&bfDesc, 0, this->buffer.GetAddressOf());
	}
	HRESULT initialize(ID3D11Device* device)
	{
		return initialize(device, data.size());
	}
	bool apply_changes(ID3D11DeviceContext* device_context)
	{
		try
		{
			D3D11_MAPPED_SUBRESOURCE mapRes;
			HRESULT hr = device_context->Map(this->buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapRes);
			COM_ERROR_IF_FAILED(hr, "Failed to update constant buffer.");
			CopyMemory(mapRes.pData, data.data(), sizeof(Type) * data.size());
			device_context->Unmap(this->buffer.Get(), 0);
		}
		catch (COMException& exp)
		{
			ErrorLogger::Log(exp);
			return false;
		}
		return true;
	}
	void set_constant_buffer(ID3D11DeviceContext* device_context)
	{
		UINT slot = Helper::get_constant_buffer_slot();
		auto buffer_address_of = GetAddressOf();

		if (Helper::is_use_in_vs())
			device_context->VSSetConstantBuffers(slot, 1, buffer_address_of);
		if (Helper::is_use_in_ps())
			device_context->PSSetConstantBuffers(slot, 1, buffer_address_of);
	}
	void set_constant_buffer(ID3D11DeviceContext* device_context, UINT slot)
	{
		auto buffer_address_of = GetAddressOf();

		if (Helper::is_use_in_vs())
			device_context->VSSetConstantBuffers(slot, 1, buffer_address_of);
		if (Helper::is_use_in_ps())
			device_context->PSSetConstantBuffers(slot, 1, buffer_address_of);
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
			device_context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapRes);
			data = (Type*)mapRes.pData;
		}
		~AutoMapUnmap()
		{
			device_context->Unmap(buffer, 0);
			data = nullptr; // CPU에서 접근하지 못하게 막아두어야 한다. 자동으로 삭제되는거 같지만 꺼두자.
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