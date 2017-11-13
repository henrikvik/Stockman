#pragma once
#include <d3d11.h>


namespace Global
{
    struct Nulls
    {
        void * nulls[32] = { nullptr };
#define MAKE_OPERATOR(type) operator type**() { return (type**)nulls; }
    MAKE_OPERATOR(ID3D11Buffer);
    MAKE_OPERATOR(ID3D11ShaderResourceView);
    MAKE_OPERATOR(ID3D11RenderTargetView);
    MAKE_OPERATOR(ID3D11UnorderedAccessView);
    MAKE_OPERATOR(ID3D11SamplerState);
#undef MAKE_OPERATOR
    };

    extern ID3D11Device * device;
    extern ID3D11DeviceContext * context;
    extern Nulls nulls;
}