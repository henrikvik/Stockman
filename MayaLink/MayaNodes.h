#pragma once

#include <Windows.h>
#include "mayaVariables.h"
#include "MayaStructs.h"

class MayaNodes
{
private:

#pragma region Variables
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

	MPlug outColor;
	MPlug color;
	MPlug diffuse;
	
	MObject data;
#pragma endregion

public:
	MayaNodes();
	~MayaNodes();
	
	void getMeshInfo(MFnMesh &mesh);
	void getTransform(MFnTransform & transform, MFnMesh & mesh);

	MPlug plugSearch(MFnDependencyNode &node, MString string);

	void getMaterial(MObject & iterator);

	void surfaceShaderMaterial(MPlug & plug);

	void getDirectionalLight();
	void getPointLight();

	void getCamera();
	void updateCamera();

	void MeshCallBack(MNodeMessage::AttributeMessage msg, MPlug & plug, MPlug & otherPlug, void * clientData);
};

