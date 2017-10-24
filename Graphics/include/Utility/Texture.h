#pragma once
#include <d3d11.h>
#include <vector>
#include <stdint.h>

class Texture
{
public:
    Texture(ID3D11Device * device, std::vector<uint8_t> & data);
    Texture(ID3D11Device * device, const char * filePath);
    virtual ~Texture();

    operator ID3D11ShaderResourceView*() { return resourceView; }
    operator ID3D11ShaderResourceView**() { return &resourceView; }
private:
    ID3D11Resource * resource;
    ID3D11ShaderResourceView * resourceView;
};
