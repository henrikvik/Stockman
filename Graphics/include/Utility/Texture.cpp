#include "Texture.h"
#include "../ThrowIfFailed.h"
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#include <comdef.h>

Texture::Texture()
{
    resource = nullptr;
    resourceView = nullptr;
}

Texture::Texture(ID3D11Device * device, std::vector<uint8_t>& data, bool useDDS)
{
    if (useDDS)
    {
        ThrowIfFailed(DirectX::CreateDDSTextureFromMemory(device, data.data(), data.size(), &resource, &resourceView));
    }
    else
    {
        ThrowIfFailed(DirectX::CreateWICTextureFromMemory(device, data.data(), data.size(), &resource, &resourceView));
    }
}

Texture::Texture(ID3D11Device * device, const char * filePath, bool useDDS)
{
    if (useDDS)
    {
        ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device, _bstr_t(filePath), &resource, &resourceView));
    }
    else
    {
        ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, _bstr_t(filePath), &resource, &resourceView));
    }
}

Texture::~Texture()
{
    resource ? resource->Release() : NULL;
    resourceView ? resourceView->Release() : NULL;
}
