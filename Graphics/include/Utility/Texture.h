#pragma once
#include <d3d11.h>
#include <vector>
#include <stdint.h>

class Texture
{
public:
    Texture();
    Texture(ID3D11Device * device, std::vector<uint8_t> & data, bool useDDS = false);
    Texture(ID3D11Device * device, const char * filePath, bool useDDS = false);
    virtual ~Texture();

    operator ID3D11ShaderResourceView*() { return resourceView; }
    operator ID3D11ShaderResourceView**() { return &resourceView; }

    Texture(Texture&) = delete;
    void operator=(Texture&) = delete;
private:
    ID3D11Resource * resource;
    ID3D11ShaderResourceView * resourceView;
};
