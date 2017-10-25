#include "Texture.h"
#include "../ThrowIfFailed.h"
#include <WICTextureLoader.h>
#include <comdef.h>

Texture::Texture()
{
    resource = nullptr;
    resourceView = nullptr;
}

Texture::Texture(ID3D11Device * device, std::vector<uint8_t>& data)
{
    ThrowIfFailed(DirectX::CreateWICTextureFromMemory(device, data.data(), data.size(), &resource, &resourceView));
}

Texture::Texture(ID3D11Device * device, const char * filePath)
{
    ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, _bstr_t(filePath), &resource, &resourceView));
}

Texture::~Texture()
{
    resource ? resource->Release() : NULL;
    resourceView ? resourceView->Release() : NULL;
}
