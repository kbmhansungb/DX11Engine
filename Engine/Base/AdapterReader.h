#pragma once

#include "../_Include.h"

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

