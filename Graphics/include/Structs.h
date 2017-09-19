#pragma once

#include <d3d11.h>
#include <SimpleMath.h>
#include <Engine/Constants.h>

#define AVG_TILE_LIGHTS 200
#define BLOCK_SIZE 16

namespace Graphics
{
    enum ModelID {
        CUBE
    };

	struct ModelInfo
	{
		size_t indexCount;
        ID3D11Buffer * indexBuffer;
        ID3D11Buffer * vertexBuffer;
	};


	struct RenderInfo
	{
		bool render;
        ModelID meshId;
		int materialId;
		DirectX::SimpleMath::Matrix translation;
	};


    struct InstanceData
    {
        DirectX::SimpleMath::Matrix translation;
    };

	// TODO: Change
#define NUM_LIGHTS 8

	struct Light {
		DirectX::SimpleMath::Vector4 positionVS;
		DirectX::SimpleMath::Vector3 positionWS;
		float range;
		DirectX::SimpleMath::Vector3 color;
		float intensity;
	};
};