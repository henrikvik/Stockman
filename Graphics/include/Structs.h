#ifndef STRUCTS_H
#define STRUCTS_H

#include <d3d11.h>
#include <SimpleMath.h>
#include <vector>


namespace Graphics
{
	struct RenderInfo
	{
		bool render;
		int meshId;
		int materialId;
		DirectX::SimpleMath::Matrix translation;
	};

	// Diffuse and Specular     [Dr][Dg][Db][S]
	// Normal and Material Id   [Nx][Ny][Nz][M]
	// Position                 [Px][Py][Pz][Py]
	struct GBuffer
	{
		ID3D11RenderTargetView * diffuseSpec;
		ID3D11RenderTargetView * normalMat;
		ID3D11RenderTargetView * position;
		ID3D11DepthStencilView * depth;
		ID3D11ShaderResourceView * diffuseSpecView;
		ID3D11ShaderResourceView * normalMatView;
		ID3D11ShaderResourceView * positionView;
		ID3D11ShaderResourceView * depthView;
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
