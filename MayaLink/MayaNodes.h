#pragma once

#include "LinkFiles.h"
#include <Windows.h>
#include "Circlebuffer.h"
#include "MayaStructs.h"

class MayaNodes
{
private:

public:
	MayaNodes();
	~MayaNodes();
	
	void getMeshInfo(MFnMesh &mesh);
	void getLight();
	void getCamera();
	void MeshCallBack(MNodeMessage::AttributeMessage msg, MPlug & plug, MPlug & otherPlug, void * clientData);
};

