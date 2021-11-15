#include "AdapterReader.h"
using namespace std;

AdapterData::AdapterData(IDXGIAdapter* pAdapter) : pAdapter(pAdapter)
{
	// 1. AdapterData를 생성할 때 IDXGIAdapter에서 desc를 가져옴. 21.11.15
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
	// 1. AdapterData를 이미 로드한적이 있다면 종료
	// 아니라면 데이터를 만들도록 함. 21.11.15
	if (adapters.size() > 0) return adapters;

	// 2. 임시 IDXGIFactor 선언 및 할당. 21.11.15
	Microsoft::WRL::ComPtr<IDXGIFactory> pFactory; 
	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(pFactory.GetAddressOf()));
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create DXGIFactory for enumerating adapters.");
		exit(-1);
	}

	// 3. 순서대로 어뎁터 데이터를 입력. 21.11.15
	IDXGIAdapter* pAdapter;
	UINT index = 0;
	while (SUCCEEDED(pFactory->EnumAdapters(index++, &pAdapter)))
	{
		adapters.push_back(AdapterData(pAdapter));
	}

	// 4. 종료. 21.11.15
	return adapters;
}