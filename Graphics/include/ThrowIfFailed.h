#pragma once

#include <exception>
#include <Windows.h>
#include <stdio.h>
#include <iostream>

// Helper class for COM exceptions
class com_exception : public std::exception
{
public:
    com_exception(HRESULT hr) : result(hr) {}

    virtual const char* what() const override
    {
        static char s_str[64] = { 0 };
        sprintf_s(s_str, "Failure with HRESULT of %08X",
            static_cast<unsigned int>(result));
        return s_str;
    }

private:
    HRESULT result;
};

// Helper utility converts D3D API failures into exceptions.
inline void ThrowIfFailedOld(HRESULT hr)
{
    if (FAILED(hr))
    {
        throw com_exception(hr);
    }
}


#define ThrowWithMessage(hr, file, line) \
{\
    if (FAILED(hr))\
    {\
        std::cerr << file << line << std::endl;\
        throw com_exception(hr);\
    }\
}\

#define ThrowIfFailed(hr) ThrowWithMessage(hr, __FILE__, __LINE__);