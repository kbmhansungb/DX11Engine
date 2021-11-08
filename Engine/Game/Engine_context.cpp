#include "Engine_context.h"

EngineContext* last_engine_context;

EngineContext::EngineContext()
{
	D3D11CreateDevice(
		this->use_adapter->pAdapter,
		D3D_DRIVER_TYPE_UNKNOWN,
		0, 0, 0, 0,
		D3D11_SDK_VERSION,
		this->Device.GetAddressOf(),
		0,
		this->Device_context.GetAddressOf()
	);

	last_engine_context = this;
}

EngineContext* EngineContext::get_instance()
{
	return last_engine_context;
}

