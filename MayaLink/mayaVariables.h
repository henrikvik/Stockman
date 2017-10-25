#pragma once
#include "LinkFiles.h"
#include "MayaStructs.h"
#include "MayaNodes.h"

#define MSSUCCESS MS::kSuccess
#define DisplayI MGlobal::displayInfo

MStatus result = MS::kSuccess;
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

CircleBuffer * circleBuffer = nullptr;
MayaNodes * MN = nullptr;

MItDag iterator(MItDag::kDepthFirst);
MItDependencyNodes dNodesIterator(MFn::kBlinn);

MObject parent;