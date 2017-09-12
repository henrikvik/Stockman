#include <Resources\TextureManager.h> 

using namespace DirectX;

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
	for (unsigned int i = 0; i < diffuseTextures.size(); i++)
	{
		delete diffuseTextures.at(i);
	}
	for (unsigned int i = 0; i < normalTextures.size(); i++)
	{
		delete normalTextures.at(i);
	}
	for (unsigned int i = 0; i < specularTextures.size(); i++)
	{
		delete specularTextures.at(i);
	}
	for (unsigned int i = 0; i < glowTextures.size(); i++)
	{
		delete glowTextures.at(i);
	}
}

void TextureManager::initilize(ID3D11Device * gDevice)
{
	this->gDevice = gDevice;
	CoInitialize((LPVOID)0);
}

void TextureManager::release()
{
	for (unsigned int i = 0; i < diffuseTextures.size(); i++)
	{
		SAFE_RELEASE(*diffuseTextures.at(i));
	}
	for (unsigned int i = 0; i < normalTextures.size(); i++)
	{
		SAFE_RELEASE(*normalTextures.at(i));
	}
	for (unsigned int i = 0; i < specularTextures.size(); i++)
	{
		SAFE_RELEASE(*specularTextures.at(i));
	}
	for (unsigned int i = 0; i < glowTextures.size(); i++)
	{
		SAFE_RELEASE(*glowTextures.at(i));
	}
}

int TextureManager::getDiffuseID(string diffuseTex)
{
	if (diffuseTex == "")
	{
		return -1;
	}
	if (this->diffuseTex.size() == 0)
	{
		this->diffuseTex.push_back(diffuseTex);
		AddDiffuseTexture(diffuseTex);
		return 0;
	}
	else
	{
		for (unsigned int i = 0; i < this->diffuseTex.size(); i++)
		{
			if (this->diffuseTex.at(i) == diffuseTex)
			{
				return i;
			}
		}
	}
	this->diffuseTex.push_back(diffuseTex);
	this->diffuseTex.shrink_to_fit();
	AddDiffuseTexture(diffuseTex);
	return (int)(this->diffuseTex.size() - 1);
}

int TextureManager::getNormalID(string normalTex)
{
	if (normalTex == "")
		return -1;
	if (this->normalTex.size() == 0)
	{
		this->normalTex.push_back(normalTex);
		AddNormalTexture(normalTex);
		return 0;
	}
	else
	{
		for (unsigned int i = 0; i < this->normalTex.size(); i++)
		{
			if (this->normalTex.at(i) == normalTex)
				return i;
		}
	}
	this->normalTex.push_back(normalTex);
	this->normalTex.shrink_to_fit();
	AddNormalTexture(normalTex);
	return (int)(this->normalTex.size() - 1);
}

int TextureManager::getSpecularID(string specularTex)
{
	if (specularTex == "")
		return -1;
	if (this->specularTex.size() == 0)
	{
		this->specularTex.push_back(specularTex);
		AddSpecularTexture(specularTex);
		return 0;
	}
	else
	{
		for (unsigned int i = 0; i < this->specularTex.size(); i++)
		{
			if (this->specularTex.at(i) == specularTex)
				return i;
		}
	}
	this->specularTex.push_back(specularTex);
	this->specularTex.shrink_to_fit();
	AddSpecularTexture(specularTex);
	return (int)(this->specularTex.size() - 1);
}

int TextureManager::getGlowID(string glowTex)
{
	if (glowTex == "")
		return -1;
	if (this->glowTex.size() == 0)
	{
		this->glowTex.push_back(glowTex);
		AddGlowTexture(glowTex);
		return 0;
	}
	else
	{
		for (unsigned int i = 0; i < this->glowTex.size(); i++)
		{
			if (this->glowTex.at(i) == glowTex)
				return i;
		}
	}
	this->glowTex.push_back(glowTex);
	this->glowTex.shrink_to_fit();
	AddGlowTexture(glowTex);
	return (int)(this->glowTex.size() - 1);
}

int TextureManager::findDiffuseID(string diffuseTex)
{
	for (unsigned int i = 0; i < this->diffuseTex.size(); i++)
	{
		if (this->diffuseTex.at(i) == diffuseTex)
		{
			return i;
		}
	}

	return -1;
}

int TextureManager::findNormalID(string normalTex)
{
	for (unsigned int i = 0; i < this->normalTex.size(); i++)
	{
		if (this->normalTex.at(i) == normalTex)
		{
			return i;
		}
	}

	return -1;
}

int TextureManager::findSpeularID(string specualrTex)
{
	for (unsigned int i = 0; i < this->specularTex.size(); i++)
	{
		if (this->specularTex.at(i) == specualrTex)
		{
			return i;
		}
	}

	return -1;
}

int TextureManager::findGloowID(string glowTex)
{
	for (unsigned int i = 0; i < this->glowTex.size(); i++)
	{
		if (this->glowTex.at(i) == glowTex)
		{
			return i;
		}
	}

	return -1;
}

ID3D11ShaderResourceView * TextureManager::GetDiffuseTexture(int diffuseID)
{
	if (diffuseID == -1)
	{
		return nullptr;
	}
	else
	{
		try
		{
			return *diffuseTextures.at(diffuseID);
		}
		catch (...)
		{
			return nullptr;
		}
	}

	return nullptr;
}

ID3D11ShaderResourceView * TextureManager::GetNormalTexture(int normalID)
{
	if (normalID == -1)
	{
		return nullptr;
	}
	else
	{
		try
		{
			return *normalTextures.at(normalID);
		}
		catch (...)
		{
			return nullptr;
		}
	}

	return nullptr;
}

ID3D11ShaderResourceView * TextureManager::GetSpecularTexture(int specularID)
{
	if (specularID == -1)
	{
		return nullptr;
	}
	else
	{
		try
		{
			return *specularTextures.at(specularID);
		}
		catch (...)
		{
			return nullptr;
		}
	}

	return nullptr;
}

ID3D11ShaderResourceView * TextureManager::GetGlowTexture(int glowID)
{
	if (glowID == -1)
	{
		return nullptr;
	}
	else
	{
		try
		{
			return *glowTextures.at(glowID);
		}
		catch (...)
		{
			return nullptr;
		}
	}

	return nullptr;
}

void TextureManager::AddDiffuseTexture(string diffuseTex)
{
	ID3D11ShaderResourceView** texture = newd ID3D11ShaderResourceView*;

	diffuseTex = (TEXTURE_PATH(L"Resources/Textures/"), diffuseTex);
	wstring widestr = wstring(diffuseTex.begin(), diffuseTex.end());
	const wchar_t* fileName = widestr.c_str();

	hr = CreateWICTextureFromFile(gDevice, fileName, nullptr, texture);

	diffuseTextures.push_back(texture);
	diffuseTextures.shrink_to_fit();
}

void TextureManager::AddNormalTexture(string normalTex)
{
	ID3D11ShaderResourceView** texture = newd ID3D11ShaderResourceView*;
	
	normalTex = (TEXTURE_PATH(L"Resources/Textures/"), normalTex);
	wstring widestr = wstring(normalTex.begin(), normalTex.end());
	const wchar_t* fileName = widestr.c_str();

	hr = CreateWICTextureFromFile(gDevice, fileName, nullptr, texture);

	normalTextures.push_back(texture);
	normalTextures.shrink_to_fit();
}

void TextureManager::AddSpecularTexture(string specularTex)
{
	ID3D11ShaderResourceView** texture = newd ID3D11ShaderResourceView*;

	specularTex = (TEXTURE_PATH(L"Resources/Textures/"), specularTex);
	wstring widestr = wstring(specularTex.begin(), specularTex.end());
	const wchar_t* fileName = widestr.c_str();

	hr = CreateWICTextureFromFile(gDevice, fileName, nullptr, texture);

	specularTextures.push_back(texture);
	specularTextures.shrink_to_fit();
}

void TextureManager::AddGlowTexture(string glowTex)
{
	ID3D11ShaderResourceView** texture = newd ID3D11ShaderResourceView*;

	glowTex = (TEXTURE_PATH(L"Resources/Textures/"), glowTex);
	wstring widestr = wstring(glowTex.begin(), glowTex.end());
	const wchar_t* fileName = widestr.c_str();

	hr = CreateWICTextureFromFile(gDevice, fileName, nullptr, texture);

	glowTextures.push_back(texture);
	glowTextures.shrink_to_fit();
}