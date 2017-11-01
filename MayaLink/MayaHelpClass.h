#pragma once

#ifndef MayaHelpClass_H
#define MayaHelpClass_H

#include "LinkFiles.h"
//#include "MStructs.h"

//#include "Circlebuffer.h"

class MayaHelpClass
{
private:

	//CircleBuffer * circleBuffer = nullptr;
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

public:
	MayaHelpClass();
	~MayaHelpClass();

	//void copy(float dest[], MFloatPoint & src);

	//void copy(float dest[], MFloatVector & src);

	//void getMeshInfo(MFnMesh &mesh);
	//void getTransform(MFnTransform & transform, MFnMesh & mesh);

	//MPlug plugSearch(MFnDependencyNode &node, MString string);

	//void getMaterial(MObject & iterator);

	//void surfaceShaderMaterial(MPlug & plug);

	//void getCamera();
};


#endif MayaHelpClass_H