#pragma once
#include <Engine/Constants.h>
#include "../Structs.h"
#include "../Datatypes.h"
#include <vector>

#include "WICTextureLoader.h"

class TextureManager
{
public:
	TextureManager();
	~TextureManager();
	
	void initilize(ID3D11Device* gDevice);
	void release();

	int getDiffuseID(string diffuseTex);
	int getNormalID(string normalTex);
	int getSpecularID(string specularTex);
	int getGlowID(string GlowTex);

	int findDiffuseID(string diffuseTex);
	int findNormalID(string normalTex);
	int findSpeularID(string specualrTex);
	int findGloowID(string glowTex);

	ID3D11ShaderResourceView* GetDiffuseTexture(int diffuseID);
	ID3D11ShaderResourceView* GetNormalTexture(int normalID);
	ID3D11ShaderResourceView* GetSpecularTexture(int specularID);
	ID3D11ShaderResourceView* GetGlowTexture(int glowID);

private:
	ID3D11Device* gDevice;
	HRESULT hr;

	vector<string> diffuseTex;
	vector<string> normalTex;
	vector<string> specularTex;
	vector<string> glowTex;

	std::vector<ID3D11ShaderResourceView**> diffuseTextures;
	std::vector<ID3D11ShaderResourceView**> normalTextures;
	std::vector<ID3D11ShaderResourceView**> specularTextures;
	std::vector<ID3D11ShaderResourceView**> glowTextures;

	void AddDiffuseTexture(string diffuseTex);
	void AddNormalTexture(string normalTex);
	void AddSpecularTexture(string specularTex);
	void AddGlowTexture(string glowTex);
};