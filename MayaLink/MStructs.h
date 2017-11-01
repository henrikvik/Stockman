#pragma once

#include "LinkFiles.h"
#include "CircleBuffer.h"

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

MItDag iterator(MItDag::kDepthFirst);
MItDependencyNodes dNodesIterator(MFn::kBlinn);

MObject parent;

#define MSSUCCESS MS::kSuccess
#define DisplayI MGlobal::displayInfo

MStatus result = MS::kSuccess;
MCallbackIdArray idList;

char * msg;

enum MessageType
{
	MMesh,
	MTransform,
	MMaterial,
	MCamera,
	MDeleteNode,
	MDirectionalLight,
	MPointLight
};


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

MPlugArray textureGroup;
MPlugArray shadingGroupArr;
MPlugArray memberConnections;
MPlugArray objectInstanceArray;
MFnDependencyNode materialNode;

//MPlug outColor;
//MPlug color;
//MPlug diffuse;

MObject data;

CircleBuffer * CB = nullptr;