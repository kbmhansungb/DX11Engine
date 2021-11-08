#pragma once
#include <string>

class StringHelper
{
public:	
	static std::wstring StringToWide(std::string str);
	static std::string WideToString(std::wstring wstr);
	static std::string GetFileExtension(const std::string& filename);
	static std::string GetFileName(const std::string& string);
	static std::string GetDirectoryFromPath(const std::string& filePath);
	static std::string ptr_to_string(void* ptr);
};

