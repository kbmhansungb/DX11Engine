#include "AdapterReader.h"
using namespace std;

AdapterData::AdapterData(IDXGIAdapter* pAdapter) : pAdapter(pAdapter)
{
	// 1. AdapterData�� ������ �� IDXGIAdapter���� desc�� ������. 21.11.15
	try
	{
		HRESULT hr = pAdapter->GetDesc(&this->desctription);		
		COM_ERROR_IF_FAILED(hr, "Failed to Get Description for IDXGIAdapter.");		
	}
	catch (COMException& exp)
	{
		ErrorLogger::Log(exp);		
	}
}

vector<AdapterData> AdapterReader::adapters;

vector<AdapterData> AdapterReader::GetAdapters()
{
	// 1. AdapterData�� �̹� �ε������� �ִٸ� ����
	// �ƴ϶�� �����͸� ���鵵�� ��. 21.11.15
	if (adapters.size() > 0) return adapters;

	// 2. �ӽ� IDXGIFactor ���� �� �Ҵ�. 21.11.15
	Microsoft::WRL::ComPtr<IDXGIFactory> pFactory; 
	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(pFactory.GetAddressOf()));
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create DXGIFactory for enumerating adapters.");
		exit(-1);
	}

	// 3. ������� ��� �����͸� �Է�. 21.11.15
	IDXGIAdapter* pAdapter;
	UINT index = 0;
	while (SUCCEEDED(pFactory->EnumAdapters(index++, &pAdapter)))
	{
		adapters.push_back(AdapterData(pAdapter));
	}

	// 4. ����. 21.11.15
	return adapters;
}