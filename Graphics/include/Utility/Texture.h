#pragma once
#include <d3d11.h>
#include <vector>
#include <stdint.h>
#include <Resources\Resources.h>
#include "../../export.h"

class GRAPHICS_API Texture
{
public:
    Texture();
    Texture(ID3D11Device * device, std::vector<uint8_t> & data, bool useDDS = false);
    Texture(const char * filePath);
    virtual ~Texture();

    operator ID3D11ShaderResourceView*() { return resourceView; }
    operator ID3D11ShaderResourceView**() { return &resourceView; }

    Texture(Texture&) = delete;
    void operator=(Texture&) = delete;
private:
    ID3D11Resource * resource;
    ID3D11ShaderResourceView * resourceView;
};
