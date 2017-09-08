#pragma once

#include <d3d11.h>
#include <SimpleMath.h>

using namespace std;

struct Float2
{
	float x;
	float y;

	Float2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
	Float2() {};

	Float2& operator=(double* pos)
	{
		this->x = float(pos[0]);
		this->y = float(pos[1]);

		return *this;
	}
};

struct Float3
{
	float x;
	float y;
	float z;

	Float3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	Float3() {};

	Float3(double* pos)
	{
		this->x = float(pos[0]);
		this->y = float(pos[1]);
		this->z = float(pos[2]);

	}

	Float3& operator=(double* pos)
	{
		this->x = float(pos[0]);
		this->y = float(pos[1]);
		this->z = float(pos[2]);

		return *this;
	}
};

struct Float4
{
	float x;
	float y;
	float z;
	float w;

	Float4(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
	Float4() {};
};

struct INT4
{
	int x;
	int y;
	int z;
	int w;

	INT4(int x, int y, int z, int w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
	INT4() {};
};

struct UINT4
{
	unsigned int x;
	unsigned y;
	unsigned z;
	unsigned w;

	UINT4(unsigned x, unsigned y, unsigned z, unsigned w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
	UINT4() {};
};


struct Double3
{
	double x;
	double y;
	double z;

	Double3() {};

	Double3(double x, double y, double z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};


struct Vertex
{
	Float3 position;
	Float3 normal;
	Float2 uv;
	Float2 biTangent;
	Float2 tangent;

	Vertex(Float3 pos, Float3 normal, Float2 uv, Float2 tangent, Float2 biTangent)
	{
		this->position.x = pos.x;
		this->position.y = pos.y;
		this->position.z = pos.z;

		this->normal.x = normal.x;
		this->normal.y = normal.y;
		this->normal.z = normal.z;

		this->uv.x = uv.x;
		this->uv.y = uv.y;

		this->tangent.x = tangent.x;
		this->tangent.y = tangent.y;

		this->biTangent.x = biTangent.x;
		this->biTangent.y = biTangent.y;

	}
	Vertex() {};

};


struct Material
{
	string materialName;
	int materialID;
	Float3 diffuseValue;
	Float3 specularValue;

	//if the id is -1, the material doesn't have a texture
	int diffuse_ID;
	int normal_ID;
	int specular_ID;
	int glow_ID;
};
struct importedMaterial
{
	string materialName;
	int materialID;
	Float3 diffuseValue;
	Float3 specularValue;

	string diffuseTex;
	string normalTex;
	string specularTex;
	string glowTex;
};

struct NodeType
{
	Float2 position;
	float width;
	unsigned int materialID;
	bool isAnimated;
	unsigned int triangleCount;
	ID3D11Buffer *vertexBuffer;
	ID3D11Buffer *indexBuffer;
	UINT VertexCount;
	UINT IndexCount;
	NodeType* nodes[4];
};