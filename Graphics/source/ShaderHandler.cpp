#include "ShaderHandler.h"
#include <comdef.h>

ShaderHandler::ShaderHandler()
{
}

ShaderHandler::~ShaderHandler()
{
	for (size_t i = 0; i < vertexShaders.size(); i++)
	{
		vertexShaders[i]->Release();
		inputLayouts[i]->Release();
	}

	for (size_t i = 0; i < geometryShaders.size(); i++)
	{
		geometryShaders[i]->Release();
	}

	for (size_t i = 0; i < pixelShaders.size(); i++)
	{
		pixelShaders[i]->Release();
	}
}

int ShaderHandler::createVertexShader(ID3D11Device* device, wchar_t* name, char* entrypoint, D3D11_INPUT_ELEMENT_DESC* desc, UINT nrOfElements)
{
	ID3DBlob* blob = nullptr;
	HRESULT hr;
	int returnVal = -1;

	hr = D3DCompileFromFile(name, nullptr, nullptr, entrypoint, "vs_5_0",
#ifdef DEBUG
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
#else
		NULL,
#endif
	  0, &blob, nullptr);

	if (SUCCEEDED(hr))
	{
		ID3D11VertexShader* vertexShader;
		hr = device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader);

		if (SUCCEEDED(hr))
		{
			ID3D11InputLayout* layout;
			device->CreateInputLayout(desc, nrOfElements, blob->GetBufferPointer(), blob->GetBufferSize(), &layout);

			if (SUCCEEDED(hr))
			{
				this->vertexShaders.push_back(vertexShader);
				this->inputLayouts.push_back(layout);
				blob->Release();


				returnVal = this->vertexShaders.size() - 1;
			}
		}
	}

	else
	{
		_com_error err(hr);
		LPCTSTR errMsg = err.ErrorMessage();
	}

	if (blob)
		blob->Release();

	return returnVal;
}

int ShaderHandler::createGeometryShader(ID3D11Device* device, wchar_t* name, char* entrypoint)
{
	ID3DBlob* blob = nullptr;
	HRESULT hr;
	int returnVal = -1;
	//REMOVE DEBUG WHEN DONE
	hr = D3DCompileFromFile(
		name, nullptr, nullptr, entrypoint, "gs_5_0",
#ifdef DEBUG
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
#else
		NULL,
#endif
		0, &blob, nullptr);

	if (SUCCEEDED(hr))
	{
		ID3D11GeometryShader* geometryShader;
		hr = device->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &geometryShader);

		if (SUCCEEDED(hr))
		{
			this->geometryShaders.push_back(geometryShader);
			blob->Release();


			returnVal = this->geometryShaders.size() - 1;
		}

	}

	if (blob)
		blob->Release();

	return returnVal;
}

int ShaderHandler::createPixelhader(ID3D11Device* device, wchar_t* name, char* entrypoint)
{
	ID3DBlob* blob = nullptr;
	HRESULT hr;
	int returnVal = -1;
	//REMOVE DEBUG WHEN DONE
	hr = D3DCompileFromFile(
		name, nullptr, nullptr, entrypoint, "ps_5_0",
#ifdef DEBUG
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
#else
		NULL,
#endif
		0, &blob, nullptr);

	if (SUCCEEDED(hr))
	{
		ID3D11PixelShader* pixelShader;
		hr = device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader);

		if (SUCCEEDED(hr))
		{
			this->pixelShaders.push_back(pixelShader);
			blob->Release();


			returnVal = this->pixelShaders.size() - 1;
		}

	}

	if (blob)
		blob->Release();

	return returnVal;
}

void ShaderHandler::setShaders(int vs, int gs, int ps, ID3D11DeviceContext* context)
{
	context->VSSetShader(vs == NO_SHADER ? nullptr : vertexShaders[vs], nullptr, 0);
	context->IASetInputLayout(vs == NO_SHADER ? nullptr : inputLayouts[vs]);

	context->GSSetShader(gs == NO_SHADER ? nullptr : geometryShaders[gs], nullptr, 0);
	context->PSSetShader(ps == NO_SHADER ? nullptr : pixelShaders[ps], nullptr, 0);
}
