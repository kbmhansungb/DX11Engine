#include "AdapterReader.h"
using namespace std;

AdapterData::AdapterData(IDXGIAdapter* pAdapter) : pAdapter(pAdapter)
{
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
	if (adapters.size() > 0) return adapters;

	Microsoft::WRL::ComPtr<IDXGIFactory> pFactory; 

	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(pFactory.GetAddressOf()));
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create DXGIFactory for enumerating adapters.");
		exit(-1);
	}

	IDXGIAdapter* pAdapter;
	UINT index = 0;

	while (SUCCEEDED(pFactory->EnumAdapters(index++, &pAdapter)))
	{
		adapters.push_back(AdapterData(pAdapter));
	}

	return adapters;
}