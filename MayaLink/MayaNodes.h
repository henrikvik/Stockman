#pragma once

#include <Windows.h>
#include "mayaVariables.h"
#include "MayaStructs.h"

class MayaNodes
{
private:

public:
	MayaNodes();
	~MayaNodes();
	
	void getMeshInfo(MFnMesh &mesh);
	void getTransform(MFnTransform & transform);
	void getLight();
	void getCamera();
	void MeshCallBack(MNodeMessage::AttributeMessage msg, MPlug & plug, MPlug & otherPlug, void * clientData);
};

