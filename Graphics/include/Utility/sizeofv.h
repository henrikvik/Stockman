#pragma once
#include <vector>

template<typename T>
inline size_t sizeofv(std::vector<T> v)
{
    return sizeof(T) * v.size();
}