#pragma once

#include "../_Include.h"

/// <summary>
/// AdapterReader에 의해 사용됨.
/// IDXGIAdapter를 보관하고, 어뎁터의 정보를 가지고 있는 데이터 클래스
/// </summary>
class AdapterData
{
public:
	AdapterData(IDXGIAdapter* pAdapter);
	IDXGIAdapter* pAdapter = nullptr;
	DXGI_ADAPTER_DESC desctription;
};

/// <summary>
/// AdapterData를 벡터로 보관하고
/// 복사본을 반환하는 메서드를 가진 클래스
/// </summary>
class AdapterReader
{
	static std::vector<AdapterData> adapters;
public:
	/// <summary>
	/// adapters의 복사본을 반환한다.
	/// </summary>
	/// <returns>어뎁터 벡터의 복사본을 반환한다.</returns>
	static std::vector<AdapterData> GetAdapters();
};

