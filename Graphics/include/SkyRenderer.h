#pragma once
#include "Resources\Shader.h"
#include "Camera.h"

class SkyRenderer
{
public:
	SkyRenderer(ID3D11Device * device);
	~SkyRenderer();

	void renderSky(ID3D11DeviceContext * context, Graphics::Camera * cam);

private:
	struct  SkyCube
	{
		ID3D11Buffer * vertexBuffer;
		ID3D11Buffer * transformBuffer;
		SkyCube(ID3D11Device * device)
		{
			DirectX::SimpleMath::Vector3 vertices[] =
			{
				// FRONT
				 { -1, -1, -1},
				 { -1,  1, -1},
				 {  1, -1, -1},
				 {  1, -1, -1},
				 { -1,  1, -1},
				 {  1,  1, -1},
				// TOP						
				 { -1,  1, -1},
				 { -1,  1,  1},
				 {  1,  1, -1},
				 {  1,  1, -1},
				 { -1,  1,  1},
				 {  1,  1,  1},
				// LEFT						
				 { -1, -1,  1},
				 { -1,  1,  1},
				 { -1, -1, -1},
				 { -1, -1, -1},
				 { -1,  1,  1},
				 { -1,  1, -1},
				// RIGHT					
				 { 1,  1,   1},
				 { 1, -1,   1},
				 { 1, -1,  -1},
				 { 1,  1,   1},
				 { 1, -1,  -1},
				 { 1,  1,  -1},
				// BACK			
				 { -1,  1,  1},
				 { -1, -1,  1},
				 {  1, -1,  1},
				 { -1,  1,  1},
				 {  1, -1,  1},
				 {  1,  1,  1},
				// BOTTOM					
				 { -1, -1,  1},
				 { -1, -1, -1},
				 {  1, -1, -1},
				 { -1, -1,  1},
				 {  1, -1, -1},
				 {  1, -1,  1}			
			};

			D3D11_SUBRESOURCE_DATA subData = {};
			subData.pSysMem = vertices;

			D3D11_BUFFER_DESC desc = { 0 };
			desc.ByteWidth = sizeof(vertices);
			desc.Usage = D3D11_USAGE_IMMUTABLE;
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

			device->CreateBuffer(&desc, &subData, &vertexBuffer);

			desc.ByteWidth = sizeof(DirectX::SimpleMath::Matrix);
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

			device->CreateBuffer(&desc, NULL, &transformBuffer);
		}

		~SkyCube()
		{
			vertexBuffer->Release();
			transformBuffer->Release();
		}
	};

	Graphics::Shader shader;
	ID3D11ShaderResourceView * srv;
	SkyCube cube;

};