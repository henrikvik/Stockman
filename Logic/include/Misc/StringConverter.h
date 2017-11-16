#ifndef STRINGCONVERTER_H
#define STRINGCONVERTER_H

#include <locale> 
#include <codecvt>

// If you need (const wchar_t*) just do c_str() on the std::wstring
std::wstring convertToWString(const std::string& str)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.from_bytes(str);
}

// If you need (const char *) just do c_str() on the std::string
std::string convertToString(const std::wstring& wstr)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.to_bytes(wstr);
}

#endif // !STRINGCONVERTER_H