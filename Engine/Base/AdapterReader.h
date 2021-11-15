#pragma once

#include "../_Include.h"

/// <summary>
/// 디바이스 정보를 가져오는 클래스,
/// 
/// </summary>
class AdapterData
{
public:
	AdapterData(IDXGIAdapter* pAdapter);
	IDXGIAdapter* pAdapter = nullptr;
	DXGI_ADAPTER_DESC desctription;
};

class AdapterReader
{
	static std::vector<AdapterData> adapters;
public:
	static std::vector<AdapterData> GetAdapters();
};

