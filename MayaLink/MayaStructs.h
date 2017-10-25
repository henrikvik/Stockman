#pragma once
#include "LinkFiles.h"

struct Material
{
	int meshAmount;
	bool texture;
	char materialName[256];
	char textureFilePath[256];
	float color[3];
	float diffuse;
};

struct Meshes
{
	char meshName[256];
};

struct Vertices
{
	float positions[3];
	float normals[3];
	float uvs[2];
};

struct TransformType
{
	char meshName[256];
	float translation[3];
	double scale[3];
	double rotation[4];
};

struct MeshType
{
	char messageType[256];
	Vertices * vertexArray;
	size_t vertexCount;
};

struct PointLight
{
	float position[3];
	float color[3];
	float intensity;
};

struct DirectionalLight
{
	float position[3];
	
	float color[3];
	float intensity;
};

struct Camera
{
	char messageType[256];
	bool perspective;
	float projectionMatrix[16];
	float translation[3];
	double rotation[4];
	float farPlane;
	float nearPlane;
	float fov;
};