#pragma once
#include <d3d11.h>
#include "../ThrowIfFailed.h"

template<typename T, size_t size = 1>
class ConstantBuffer
{
public:
    ConstantBuffer(ID3D11Device * device);
    ~ConstantBuffer();

    T* map(ID3D11DeviceContext * context);
    void unmap(ID3D11DeviceContext * context);
    void write(ID3D11DeviceContext * context, T* data, UINT size);

    operator ID3D11Buffer*() { return cbuffer; }
    operator ID3D11Buffer**() { return &cbuffer; }

private:
    ID3D11Buffer * cbuffer;
};

template<typename T, size_t size>
inline ConstantBuffer<T, size>::ConstantBuffer(ID3D11Device * device)
{
    D3D11_BUFFER_DESC desc = {};
    desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
    desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
    desc.ByteWidth = max(sizeof(T) * size, 16);

    ThrowIfFailed(device->CreateBuffer(&desc, nullptr, &cbuffer));
}

template<typename T, size_t size>
inline ConstantBuffer<T, size>::~ConstantBuffer()
{
    cbuffer->Release();
}

template<typename T, size_t size>
inline T * ConstantBuffer<T, size>::map(ID3D11DeviceContext * context)
{
    D3D11_MAPPED_SUBRESOURCE sub = {};
    context->Map(cbuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &sub);
    return (T*)sub.pData;
}

template<typename T, size_t size>
inline void ConstantBuffer<T, size>::unmap(ID3D11DeviceContext * context)
{
    context->Unmap(cbuffer, 0);
}

template<typename T, size_t size>
inline void ConstantBuffer<T, size>::write(ID3D11DeviceContext * context, T * data, UINT size)
{
    D3D11_MAPPED_SUBRESOURCE sub = {};
    context->Map(cbuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &sub);
    memcpy(sub.pData, data, size);
    context->Unmap(cbuffer, 0);
}
