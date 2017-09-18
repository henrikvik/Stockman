#ifndef STRUCTS_H
#define STRUCTS_H

#include <d3d11.h>
#include <SimpleMath.h>
#include <Engine/Constants.h>
#include <vector>

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

	// Diffuse and Specular     [Dr][Dg][Db][S]
	// Normal and Material Id   [Nx][Ny][Nz][M]
	// Position                 [Px][Py][Pz][Py]
	struct GBuffer
	{
		ID3D11RenderTargetView *   diffuseSpec;
		ID3D11RenderTargetView *   normalMat;
		ID3D11RenderTargetView *   position;
		ID3D11DepthStencilView *   depth;
		ID3D11ShaderResourceView * diffuseSpecView;
		ID3D11ShaderResourceView * normalMatView;
		ID3D11ShaderResourceView * positionView;
		ID3D11ShaderResourceView * depthView;

        void Release()
        {
            SAFE_RELEASE(diffuseSpec);
            SAFE_RELEASE(normalMat);
            SAFE_RELEASE(position);
            SAFE_RELEASE(depth);
            SAFE_RELEASE(diffuseSpecView);
            SAFE_RELEASE(normalMatView);
            SAFE_RELEASE(positionView);
            SAFE_RELEASE(depthView);
        }
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
		std::vector<ButtonInfo*> m_buttons;		//< List of buttons on current menu
		std::string m_menuTexture;			//< file path for the menu background ska bytas till enums
	};
};
#endif // !STRUCTS_H