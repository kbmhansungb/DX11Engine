#pragma once

#include "_Include.h"

// �����ڿ��� ������ ���ְ� ����ϱ� ���ؼ��� ����ƽ���� �ִ� ���ۿ� ����.
// �̱��� ������ �̿��Ϸ� ������ ��ӹ޾Ƽ� ����ϴٷ� ����.

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