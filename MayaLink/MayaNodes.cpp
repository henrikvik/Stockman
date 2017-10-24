#include "MayaNodes.h"

MayaNodes::MayaNodes()
{

}

MayaNodes::~MayaNodes()
{

}

void copy(float dest[], MFloatPoint & src)
{
	dest[0] = src.x;
	dest[1] = src.y;
	dest[2] = src.z;
}

void copy(float dest[], MFloatVector & src)
{
	dest[0] = src.x;
	dest[1] = src.y;
	dest[2] = src.z;
}

void MayaNodes::getMeshInfo(MFnMesh & mesh)
{
	mesh.getPoints(points, MSpace::kObject);
	mesh.getUVs(U,V, 0);
	mesh.getAssignedUVs(uvCounts, uvIDS);

	mesh.getTriangleOffsets(triangleCountsOffsets, triangleIndices);
	mesh.getVertices(vertexCount, polyVerticesID);
	mesh.getNormals(normals, MSpace::kObject);

	vertices.resize(triangleIndices.length);

	mesh.getNormalIds(normalCount, normalList);

	for (unsigned int i = 0; i < triangleIndices.length(); i++)
	{
		copy(vertices[i].positions, points[polyVerticesID[triangleIndices[i]]]);
		copy(vertices[i].normals, normals[normalList[triangleIndices[i]]]);

		vertices[i].uvs[0] = U[uvIDS[triangleIndices[i]]];
		vertices[i].uvs[1] = V[uvIDS[triangleIndices[i]]];
	}

	offset = 0;

	int Type = MessageType::MayaMesh;
	memcpy(msg, &Type, sizeof(int));
	offset += sizeof(int);

	memcpy(&meshType, mesh.name().asChar(), sizeof(const char[256]));
	meshType.vertexArray = nullptr;
	meshType.vertexCount = vertices.size();

	memcpy((msg + offset), &meshType, sizeof(MeshType));
	offset += sizeof(MeshType); 

	memcpy(msg + offset, vertices.data(), sizeof(Vertices) * vertices.size());
	offset += sizeof(vertices) * vertices.size();

	circleBuffer->push(msg, offset);
}

void MayaNodes::getLight()
{

}

void MayaNodes::getCamera()
{
	camView = M3dView::active3dView();
	camView.getCamera(cameraPath);

	MFnCamera fnCam(cameraPath);
	MSpace::Space space = MSpace::kWorld;

	MVector up = fnCam.upDirection(space);
	MVector right = fnCam.rightDirection(space);

	if (fnCam.isOrtho())
		camera.perspective = false;
	else
		camera.perspective = true;
}

void MayaNodes::MeshCallBack(MNodeMessage::AttributeMessage msg, MPlug &plug, MPlug &otherPlug, void* clientData)
{
	if (msg & MNodeMessage::AttributeMessage::kAttributeSet)
	{
		MFnMesh mesh(plug.node(), &res);
		if (res == MS::kSuccess)
		{
			getMeshInfo(mesh);
		}
	}
}

