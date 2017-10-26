#pragma once
#include "LinkFiles.h"
#include "MayaStructs.h"
#include "MayaNodes.h"
#include "Circlebuffer.h"

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

CircleBuffer * circleBuffer = nullptr;
MayaNodes * MN = nullptr;

MItDag iterator(MItDag::kDepthFirst);
MItDependencyNodes dNodesIterator(MFn::kBlinn);

MObject parent;