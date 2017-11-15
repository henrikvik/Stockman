#pragma once
#include "ShaderResource.h"
#include "../Device.h"

class PingPongBuffer
{
public:
    PingPongBuffer(UINT width, UINT height, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM):
        ping(width, height, format),
        pong(width, height, format)
    {
        pingActive = true;
    };

    virtual ~PingPongBuffer() {};

    operator ID3D11RenderTargetView*const*() const
    {
        if (pingActive)
            return ping;

        else
            return pong;
    };

    operator ID3D11RenderTargetView*() const
    {
        if (pingActive)
            return ping;

        else
            return pong;
    };

    operator ID3D11UnorderedAccessView*const*() const
   {
        if (pingActive)
            return ping;

        else
            return pong;
    };

    operator ID3D11UnorderedAccessView*() const
    {
        if (pingActive)
            return ping;

        else
            return pong;
    };

    operator ID3D11ShaderResourceView*const*() const
    {
        if (!pingActive)
            return ping;

        else
            return pong;
    };

    operator ID3D11ShaderResourceView*() const
    {
        if (!pingActive)
            return ping;

        else
            return pong;
    };

    void swap()
    {
        pingActive = !pingActive;
    }

    void clear() const
    {
        static float color[4] = { 0 };
        Global::context->ClearRenderTargetView(ping, color);
        Global::context->ClearRenderTargetView(pong, color);
    }

    void reset()
    {
        pingActive = true;
    }
    
private:
    ShaderResource ping;
    ShaderResource pong;

    bool pingActive;
};