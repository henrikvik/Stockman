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
		BUSH,
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

    struct RenderDebugInfo
    {
        bool useDepth;
        DirectX::SimpleMath::Color color;
        std::vector<DirectX::SimpleMath::Vector3>* points;
        D3D11_PRIMITIVE_TOPOLOGY topology;
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
	struct InstanceData
	{
		DirectX::SimpleMath::Matrix translation;
	};

	struct ButtonInfo
	{
		DirectX::SimpleMath::Rectangle m_rek;
		DirectX::SimpleMath::Vector2 m_texCoordStart;
		DirectX::SimpleMath::Vector2 m_texCoordEnd;
		std::string m_texture;
		bool pressed;
	};

	struct MenuInfo
	{
		std::vector<ButtonInfo> m_buttons;		//< List of buttons on current menu
		std::string m_menuTexture;			//< file path for the menu background ska bytas till enums
	};

    struct TriangleVertex
    {
        float x, y, z;
        float u, v;
    };
    enum Font
    {
        SMALL,
        MEDUIM,
        LARGE
    };
    struct TextString
    {
        std::wstring text;
        DirectX::SimpleMath::Vector2 pos;
        DirectX::SimpleMath::Color color;
        Font font;
    };

};