#pragma once

#include <d3d11.h>
#include <SimpleMath.h>
#include <Engine/Constants.h>
#include <vector>
#include <string>

#define AVG_TILE_LIGHTS 200
#define BLOCK_SIZE 16

namespace Graphics
{
    enum ModelID {
        CUBE,
		SPHERE,
        CROSSBOW,
		AMMOBOX,
		CUTTLERY,
		JUMPPAD,
		ENEMYGRUNT,
		GRAPPLEPOINT,
		GRASS,
		SKY_SPHERE,
		BUSH,
		CLOUDS,
		MEGAGRASS

    };

	struct ModelInfo
	{
		size_t indexCount;
        ID3D11Buffer * indexBuffer;
        ID3D11Buffer * vertexBuffer;
		ID3D11ShaderResourceView * diffuseMap;
		ID3D11ShaderResourceView * normalMap;
		ID3D11ShaderResourceView * specularMap;
	};

	struct RenderInfo
	{
		bool render;
        ModelID meshId;
		int materialId;
		DirectX::SimpleMath::Matrix translation;
		bool backFaceCulling = true;
	};

	struct FoliageRenderInfo
	{
		bool render;
		ModelID meshId;
		DirectX::SimpleMath::Matrix translation;
		bool backFaceCulling = false;
	};

    struct RenderDebugInfo
    {
        bool useDepth;
        DirectX::SimpleMath::Color color;
        std::vector<DirectX::SimpleMath::Vector3>* points;
        D3D11_PRIMITIVE_TOPOLOGY topology;
    };


#define MAX_LIGHTS 128

	struct Light {
		DirectX::SimpleMath::Vector4 positionVS;
		DirectX::SimpleMath::Vector3 positionWS;
		float range;
		DirectX::SimpleMath::Vector3 color;
		float intensity;

		Light(){}

		Light(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 color, float range = 1, float intensity = 1)
		{
			positionWS = pos;
			this->range = range;
			this->color = color;
			this->intensity = intensity;
		}
	};
	struct InstanceData
	{
		DirectX::SimpleMath::Matrix translation;
		DirectX::SimpleMath::Matrix invTranspose;
	};

	struct ButtonInfo
	{
		DirectX::SimpleMath::Rectangle m_rek;
		DirectX::SimpleMath::Vector2 m_texCoordStart;
		DirectX::SimpleMath::Vector2 m_texCoordEnd;
        float activeoffset;
        int textureIndex;
		bool pressed;
	};

	struct MenuInfo
	{
		std::vector<ButtonInfo> m_buttons;		//< List of buttons on current menu
		int m_menuTexture;			
	};

    struct TriangleVertex
    {
        float x, y, z;
        float u, v;
    };
    enum Font
    {
        SMALL,
        MEDIUM,
        LARGE
    };
    struct TextString
    {
        std::wstring text;
        DirectX::SimpleMath::Vector2 pos;
        DirectX::SimpleMath::Color color;
        Font font;
    };

    struct HUDInfo
    {
        int hp;
        int cuttleryAmmo[2];
        int iceAmmo[2];
        bool sledge;
        float cd;
        UINT score;
        int wave;
        int enemiesRemaining;
        float timeRemaining;
    };

};