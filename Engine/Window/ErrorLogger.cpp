#include "ErrorLogger.h"
#include <comdef.h>
using namespace std;

void ErrorLogger::Log(string msg)
{
	string error_msg = "Error: " + msg;
	MessageBoxA(nullptr, error_msg.c_str(), "Error", MB_ICONERROR);
}

void ErrorLogger::Log(HRESULT hr, string msg)
{
	_com_error error(hr);
	wstring error_msg = L"Error: " + StringHelper::StringToWide(msg) +
		L"\n" + error.ErrorMessage();
	MessageBoxW(nullptr, error_msg.c_str(), L"Error", MB_ICONERROR);
}

void ErrorLogger::Log(HRESULT hr, wstring msg)
{
	_com_error error(hr);
	wstring error_msg = L"Error: " + msg +
		L"\n" + error.ErrorMessage();
	MessageBoxW(nullptr, error_msg.c_str(), L"Error", MB_ICONERROR);
}

void ErrorLogger::Log(COMException& exp)
{
	wstring error_msg = exp.what();
	MessageBoxW(nullptr, error_msg.c_str(), L"Error", MB_ICONERROR);
}
