#pragma once
#include <comdef.h>
#include "StringHelper.h"

#define COM_ERROR_IF_FAILED(hr,msg) if(FAILED(hr)) throw COMException(hr,msg,__FILE__,__FUNCTION__,__LINE__)

class COMException
{
	std::wstring whatMsg;
public:
	COMException(HRESULT hr, const std::wstring& msg, const std::string& file, const std::string& fuction, int line)
	{
		_com_error error(hr);
		whatMsg = L"Msg: " + msg + L"\n";
		whatMsg += error.ErrorMessage();
		whatMsg += L"\nFile: " + StringHelper::StringToWide(file);
		whatMsg += L"\nFuction: " + StringHelper::StringToWide(fuction);
		whatMsg += L"\nLine: " + StringHelper::StringToWide(std::to_string(line));
	}

	COMException(HRESULT hr, const std::string& msg, const std::string& file, const std::string& fuction, int line)
	{
		_com_error error(hr);
		whatMsg = L"Msg: " + StringHelper::StringToWide(msg) + L"\n";
		whatMsg += error.ErrorMessage();
		whatMsg += L"\nFile: " + StringHelper::StringToWide(file);
		whatMsg += L"\nFuction: " + StringHelper::StringToWide(fuction);
		whatMsg += L"\nLine: " + StringHelper::StringToWide(std::to_string(line));
	}

	const wchar_t* what() const
	{		
		return whatMsg.c_str();
	}
};
