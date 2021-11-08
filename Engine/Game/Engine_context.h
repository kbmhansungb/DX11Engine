#pragma once

#include "_Include.h"

// 생성자에서 변수를 안주고 사용하기 위해서는 스테틱으로 주는 수밖에 없다.
// 싱글톤 패턴을 이용하려 했으나 상속받아서 사용하다록 하자.

class EngineContext
{
public:
	std::vector<AdapterData> adapters = AdapterReader::GetAdapters();
	AdapterData* use_adapter = &adapters[0];

	Microsoft::WRL::ComPtr<ID3D11Device>		Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> Device_context;
protected:
	EngineContext();
public:
	static EngineContext* get_instance();
};