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

MStatus res = MS::kSuccess;
MCallbackIdArray idList;
size_t offset = 0;

MFloatPointArray points;
std::vector<Vertices> vertices;
MIntArray vertexCount,
polyVerticesID,
normalList,
normalCount;
MFloatArray U, V;
MIntArray uvCounts, uvIDS;
MFloatVectorArray normals;
MIntArray triangleCountsOffsets, triangleIndices;

MFloatMatrix projMatrix;
MObject parentCamera, parentMesh;

std::vector<Meshes> meshVector;
Meshes meshStruct;

M3dView camView;
MDagPath cameraPath;

MVector vertexNormals;

TransformType transformType;
Camera camera;
MeshType meshType;
Material material;

char * msg;

enum MessageType { MayaMesh, MayaCamera, MayaTransform, MayaMaterial, MayaNodeDelete, MayaLight };

CircleBuffer * circleBuffer;