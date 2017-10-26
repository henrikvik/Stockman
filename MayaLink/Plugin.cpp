#include "Plugin.h"

Plugin::Plugin()
{

}

Plugin::~Plugin()
{

}

EXPORT MStatus initializePlugin(MObject obj)
{
	int messageSize = (1 << 20) / 4;
	msg = new char[messageSize];
	circleBuffer = new CircleBuffer("Buffer", 1 << 20, true, 256);

	MFnPlugin myPlugin(obj, "Maya Modelviewer", "1.0", "Any", &result);

	if (MFAIL(result))
		CHECK_MSTATUS(result);


	while (!iterator.isDone() == true)
	{
		if (iterator.currentItem().hasFn(MFn::kMesh))
		{
			MFnMesh mesh(iterator.currentItem());
			MN->getMeshInfo(mesh);
			MCallbackId meshID = MNodeMessage::addAttributeChangedCallback(iterator.currentItem(), , &result);
			
			
			if (parent.hasFn(MFn::kTransform))
			{
				MCallbackId transID = MNodeMessage::addAttributeChangedCallback(parent, , &result);

				MFnTransform transform(parent);
				
			}
			parent = iterator.currentItem();
			iterator.next();
		}

	}
	return result;
}

EXPORT MStatus uninitializePlugin(MObject obj)
{
	circleBuffer->setHeadPosition(0);
	circleBuffer->setTailPosition(0);
	delete circleBuffer;
	delete[] msg;

	MFnPlugin plugin(obj);

	DisplayI("Succesfully unloaded Maya Modelviewer!");

	MMessage::removeCallbacks(idList);

	return MSSUCCESS;
}