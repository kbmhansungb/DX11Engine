#include "StringHelper.h"
using namespace std;

wstring StringHelper::StringToWide(string str)
{
    wstring wide_string(str.begin(), str.end());
    return wide_string;
}

string StringHelper::WideToString(wstring wstr)
{
    return string(wstr.begin(), wstr.end());
}

string StringHelper::GetFileExtension(const string& filename)
{
    size_t off = filename.find_last_of('.');
    if (off == string::npos)
    {
        return {};
    }
    return string(filename.substr(off+1));
}
string StringHelper::GetFileName(const string& filePath)
{
    size_t off1 = filePath.find_last_of('\\');
    size_t off2 = filePath.find_last_of('/');

    if (off1 == string::npos && off2 == string::npos)
    {
        return "";
    }
    if (off1 == string::npos)
    {
        return filePath.substr(0, off2);
    }
    if (off2 == string::npos)
    {
        return filePath.substr(0, off1);
    }

    return filePath.substr(max(off1, off2));
}
string StringHelper::GetDirectoryFromPath(const string& filePath)
{
    size_t off1 = filePath.find_last_of('\\');
    size_t off2 = filePath.find_last_of('/');
    
    if (off1 == string::npos && off2 == string::npos)
    {
        return "";
    }
    if (off1 == string::npos)
    {
        return filePath.substr(0, off2);
    }
    if (off2 == string::npos)
    {
        return filePath.substr(0, off1);
    }

    return filePath.substr(0,max(off1,off2));
}

string StringHelper::ptr_to_string(void* ptr)
{
    int int_ptr = (int)ptr;
    string str = to_string(int_ptr);
    return str;
}
