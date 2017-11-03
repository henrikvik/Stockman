#include "Texture.h"
#include "../ThrowIfFailed.h"
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#include <comdef.h>
#include "../Device.h"

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

Texture::Texture(const char * filePath)
{
    ThrowIfFailed(DirectX::CreateWICTextureFromFile(Global::device, _bstr_t(filePath), &resource, &resourceView));
}


Texture::~Texture()
{
    resource ? resource->Release() : NULL;
    resourceView ? resourceView->Release() : NULL;
}
