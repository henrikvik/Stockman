#pragma once
#include <d3d11.h>
#include "../ThrowIfFailed.h"
#include "../../../Engine/Constants.h"
class ShaderResource
{
public:
	ShaderResource(ID3D11Device * device, UINT width, UINT height);
	~ShaderResource();

	operator ID3D11RenderTargetView*() const { return renderTarget; }
	operator ID3D11RenderTargetView*const*() const { return &renderTarget; }
	operator ID3D11ShaderResourceView*() const { return shaderResource; }
	operator ID3D11ShaderResourceView*const*() const { return &shaderResource; }
	operator ID3D11UnorderedAccessView*() const { return unorderedAccess; }
	operator ID3D11UnorderedAccessView*const*() const { return &unorderedAccess; }

private:
	ID3D11RenderTargetView * renderTarget;
	ID3D11ShaderResourceView * shaderResource;
	ID3D11UnorderedAccessView * unorderedAccess;
};
