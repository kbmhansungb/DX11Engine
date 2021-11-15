#pragma once

#include "../_Include.h"

/// <summary>
/// AdapterReader�� ���� ����.
/// IDXGIAdapter�� �����ϰ�, ����� ������ ������ �ִ� ������ Ŭ����
/// </summary>
class AdapterData
{
public:
	AdapterData(IDXGIAdapter* pAdapter);
	IDXGIAdapter* pAdapter = nullptr;
	DXGI_ADAPTER_DESC desctription;
};

/// <summary>
/// AdapterData�� ���ͷ� �����ϰ�
/// ���纻�� ��ȯ�ϴ� �޼��带 ���� Ŭ����
/// </summary>
class AdapterReader
{
	static std::vector<AdapterData> adapters;
public:
	/// <summary>
	/// adapters�� ���纻�� ��ȯ�Ѵ�.
	/// </summary>
	/// <returns>��� ������ ���纻�� ��ȯ�Ѵ�.</returns>
	static std::vector<AdapterData> GetAdapters();
};

