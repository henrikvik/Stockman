#pragma once
#include "../ThrowIfFailed.h"
#include <Engine\Constants.h>
#include "../Device.h"
#include <functional>

enum class CpuAccess
{
    None = 0,
    Read = 1 << 0,
    Write = 1 << 1,
    ReadWrite = Read | Write
};

template<typename T>
class StructuredBuffer
{
public:
#pragma region Constructors/Destructors
    StructuredBuffer(CpuAccess access, size_t count, T* ptr = nullptr);
    StructuredBuffer(ID3D11Device *device, CpuAccess access, size_t count, T* ptr = nullptr);
    ~StructuredBuffer();
#pragma endregion

#pragma region Public Functions
    void CopyTo(ID3D11DeviceContext *cxt, StructuredBuffer<T> *other);
    T* map(ID3D11DeviceContext *cxt);
    void unmap(ID3D11DeviceContext *cxt);
    void write(ID3D11DeviceContext * context, T * data, UINT size);
    void write(std::function<void(T*)> writeFunction);
#pragma endregion

#pragma region Getters and Setters
    inline ID3D11Buffer *getBuffer() const { return m_Buffer; }
    inline ID3D11ShaderResourceView *getSRV() const { return m_SRV; }
    inline ID3D11UnorderedAccessView *getUAV() const { return m_UAV; }
#pragma endregion

#pragma region Implicit Conversion Operators
    operator ID3D11Buffer*()              const { return m_Buffer; }
    operator ID3D11ShaderResourceView*()  const { return m_SRV; }
    operator ID3D11UnorderedAccessView*() const { return m_UAV; }

    operator ID3D11Buffer*const*()              const { return &m_Buffer; }
    operator ID3D11ShaderResourceView*const*()  const { return &m_SRV; }
    operator ID3D11UnorderedAccessView*const*() const { return &m_UAV; }
#pragma endregion

private:
    ID3D11Buffer              *m_Buffer;
    ID3D11ShaderResourceView  *m_SRV;
    ID3D11UnorderedAccessView *m_UAV;
};

#pragma region Constructors/Destructors
template<typename T>
StructuredBuffer<T>::StructuredBuffer(CpuAccess access, size_t count, T * ptr)
    : StructuredBuffer(Global::device, access, count, ptr)
{
}
template<typename T>
inline StructuredBuffer<T>::StructuredBuffer(ID3D11Device * device, CpuAccess access, size_t count, T * ptr)
{
    m_Buffer = nullptr;
    m_SRV = nullptr;
    m_UAV = nullptr;

    D3D11_BUFFER_DESC desc = {};
    desc.ByteWidth = (UINT)(sizeof(T) * count);

    if ((int)access & (int)CpuAccess::Read)
    {
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
        desc.Usage = D3D11_USAGE_STAGING;
    }
    else if ((int)access & (int)CpuAccess::Write)
    {
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.Usage = D3D11_USAGE_DYNAMIC;
    }
    else 
    {
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
    }

    desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    desc.StructureByteStride = sizeof(T);

    if (ptr) 
    {
        D3D11_SUBRESOURCE_DATA data = {};
        data.pSysMem = ptr;

        ThrowIfFailed(device->CreateBuffer(&desc, &data, &m_Buffer));
    }
    else 
    {
        ThrowIfFailed(device->CreateBuffer(&desc, nullptr, &m_Buffer));
    }

    if (desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
    {
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = DXGI_FORMAT_UNKNOWN;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
        srvDesc.Buffer.NumElements = (UINT)count;

        ThrowIfFailed(device->CreateShaderResourceView(m_Buffer, &srvDesc, &m_SRV));
    }

    if (desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
    {
        D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
        uavDesc.Format = DXGI_FORMAT_UNKNOWN;
        uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
        uavDesc.Buffer.NumElements = (UINT)count;

        ThrowIfFailed(device->CreateUnorderedAccessView(m_Buffer, &uavDesc, &m_UAV));
    }
}

template<typename T>
inline StructuredBuffer<T>::~StructuredBuffer()
{
    SAFE_RELEASE(m_Buffer);
    SAFE_RELEASE(m_UAV);
    SAFE_RELEASE(m_SRV);
}
#pragma endregion

#pragma region Public Functions
template<typename T>
inline void StructuredBuffer<T>::CopyTo(ID3D11DeviceContext * cxt, StructuredBuffer<T>* other) {
    cxt->CopyResource(other->getBuffer(), this->getBuffer());
}

template<typename T>
inline T * StructuredBuffer<T>::map(ID3D11DeviceContext * cxt) {
    D3D11_MAPPED_SUBRESOURCE data = {};
    cxt->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);

    return static_cast<T*>(data.pData);
}

template<typename T>
inline void StructuredBuffer<T>::unmap(ID3D11DeviceContext * cxt) {
    cxt->Unmap(m_Buffer, 0);
}

template<typename T>
inline void StructuredBuffer<T>::write(ID3D11DeviceContext * context, T * data, UINT size)
{
    memcpy(map(context), data, size);
    unmap(context);
}

template<typename T>
inline void StructuredBuffer<T>::write(std::function<void(T*)> writeFunction)
{
    writeFunction(map(Global::context));
    unmap(Global::context);
}

#pragma endregion