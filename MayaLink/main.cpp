#pragma once

#include "Windows.h"
#include "LinkFiles.h"
#include "MStructs.h"
#include <maya/MFnPlugin.h>
#include "ImporterEngine.h"

void getMeshInfo(MFnMesh & mesh)
{
	MGlobal::displayInfo("test1: ");

	mesh.getPoints(points, MSpace::kObject);
	mesh.getUVs(U, V, 0);
	mesh.getAssignedUVs(uvCounts, uvIDS);

	mesh.getTriangleOffsets(triangleCountsOffsets, triangleIndices);
	mesh.getVertices(vertexCount, polyVerticesID);
	mesh.getNormals(normals, MSpace::kObject);

	vertices.resize(triangleIndices.length());

	mesh.getNormalIds(normalCount, normalList);

	for (unsigned int i = 0; i < triangleIndices.length(); i++)
	{
		cout << "Vertex Info: " << i << endl;

		vertices.at(i).positions[0] = points[polyVerticesID[triangleIndices[i]]].x;
		vertices.at(i).positions[1] = points[polyVerticesID[triangleIndices[i]]].y;
		vertices.at(i).positions[2] = points[polyVerticesID[triangleIndices[i]]].z;

		cout << "Vertex positions: " << endl;
		cout << "X: " << vertices.at(i).positions[0];
		cout << " Y: " << vertices.at(i).positions[1] ;
		cout << " Z: " <<vertices.at(i).positions[2] << endl;


		vertices.at(i).normals[0] = points[normalList[triangleIndices[i]]].x;
		vertices.at(i).normals[1] = points[normalList[triangleIndices[i]]].y;
		vertices.at(i).normals[2] = points[normalList[triangleIndices[i]]].z;

		cout << "Vertex normal: " << endl;
		cout << "X: " << vertices.at(i).normals[0];
		cout << " Y: " << vertices.at(i).normals[1];
		cout << " Z: " << vertices.at(i).normals[2] << endl;


		vertices.at(i).uvs[0] = U[uvIDS[triangleIndices[i]]];
		vertices.at(i).uvs[1] = V[uvIDS[triangleIndices[i]]];

		cout << "Vertex UV: " << endl;
		cout << "U: " << vertices.at(i).uvs[0];
		cout << " V: " << vertices.at(i).uvs[2] << endl;
		
	}

#pragma region CopyRegion
	offset = 0;

	MGlobal::displayInfo("test12: ");

	int Type = MessageType::MMesh;
	memcpy(msg, &Type, sizeof(int));
	offset += sizeof(int);

	memcpy(&meshType, mesh.name().asChar(), sizeof(const char[256]));
	meshType.vertexArray = nullptr;
	meshType.vertexCount = vertices.size();

	MGlobal::displayInfo("test12: ");

	memcpy((msg + offset), &meshType, sizeof(MeshType));
	offset += sizeof(MeshType);

	memcpy(msg + offset, vertices.data(), sizeof(Vertices) * vertices.size());
	offset += sizeof(vertices) * vertices.size();
	MGlobal::displayInfo("test13: ");

	CB->send(msg, offset);
#pragma endregion

}

void meshCallback(
	MNodeMessage::AttributeMessage message,
	MPlug &plug, MPlug &other, void *clientData)
{
	if (message & MNodeMessage::AttributeMessage::kAttributeSet)
	{
		MFnMesh mesh(plug.node(), &result);

		if (result == MS::kSuccess)
		{
			getMeshInfo(mesh);
			
		}
	}
}

//
//void getTransform(MFnTransform & transform, MFnMesh &mesh)
//{
//	transform.getScale(transformType.scale);
//	transform.getRotationQuaternion
//	(
//		transformType.rotation[0],
//		transformType.rotation[1],
//		transformType.rotation[2],
//		transformType.rotation[3]
//	);
//
//	transformType.translation[0] = transform.getTranslation(MSpace::kTransform).x;
//	transformType.translation[1] = transform.getTranslation(MSpace::kTransform).y;
//	transformType.translation[2] = transform.getTranslation(MSpace::kTransform).z;
//
//	offset = 0;
//
//	int type = MessageType::MTransform;
//	memcpy(msg, &type, sizeof(type));
//
//	offset += sizeof(type);
//
//	memcpy(&transformType.meshName, mesh.name().asChar(), sizeof(transformType.meshName));
//	memcpy((msg + offset), &transformType, sizeof(TransformType));
//
//	offset += sizeof(TransformType);
//
//	//circleBuffer->push(msg, offset);
//
//}
//
//MPlug plugSearch(MFnDependencyNode &node, MString string)
//{
//	return node.findPlug(string);
//}
//
//void getMaterial(MObject &iterator)
//{
//	MFnDependencyNode materialNode(iterator);
//
//	DisplayI("Material name: " + materialNode.name());
//
//	MPlug outColor = plugSearch(materialNode, "Outcolor");
//	MPlug color = plugSearch(materialNode, "color");
//	MPlug diffuse = plugSearch(materialNode, "diffuse");
//
//	color.connectedTo(textureGroup, true, false, &result);
//
//	color.getValue(data);
//	MFnNumericData nData(data);
//	nData.getData
//	(
//		material.color[0],
//		material.color[1],
//		material.color[2]
//	);
//	diffuse.getValue(material.diffuse);
//	material.texture = false;
//
//	for (int i = 0; i < textureGroup.length(); i++)
//	{
//		MString file;
//		MFnDependencyNode textureNode(textureGroup[i].node());
//
//		plugSearch(textureNode, "fileTextureName").getValue(file);
//		memcpy(&material.textureFilePath, file.asChar(), sizeof(material.textureFilePath));
//
//		material.texture = true;
//	}
//
//#pragma region Copyregion
//	offset = 0;
//
//	int Type = MessageType::MMaterial;
//	memcpy(msg, &Type, sizeof(int));
//	offset += sizeof(int);
//
//	memcpy(&material.materialName, materialNode.name().asChar(), sizeof(const char[256]));
//	memcpy((msg + offset), &material, sizeof(Material));
//	offset += sizeof(Material);
//
//	memcpy(msg + offset, meshVector.data(), sizeof(Meshes) * meshVector.size());
//	offset += sizeof(Meshes) * meshVector.size();
//
//	//circleBuffer->push(msg, offset);
//	meshVector.clear();
//#pragma endregion
//
//}
//
//void surfaceShaderMaterial(MPlug &Plug)
//{
//	Plug.connectedTo(shadingGroupArr, false, true, &result);
//
//	for (int i = 0; i < shadingGroupArr.length(); i++)
//	{
//		if (shadingGroupArr[i].node().hasFn(MFn::kShadingEngine))
//		{
//			MFnDependencyNode shadingNode = shadingGroupArr[i].node();
//
//
//		}
//	}
//}
//
//void getCamera()
//{
//	camView = M3dView::active3dView();
//	camView.getCamera(cameraPath);
//
//	MFnCamera fnCam(cameraPath);
//	MSpace::Space space = MSpace::kWorld;
//
//	MVector up = fnCam.upDirection(space);
//	MVector right = fnCam.rightDirection(space);
//
//	if (fnCam.isOrtho())
//		camera.perspective = false;
//	else
//		camera.perspective = true;
//}
//
//void updateCamera()
//{
//
//}
bool callBackStatus(MStatus result, MCallbackId plugId)
{
	if (result == MS::kSuccess)
	{
		idList.append(plugId);
		MGlobal::displayInfo("Mesh Callback is a success");
		return true;
	}
	else
	{
		MGlobal::displayInfo("Mesh Callback failed");
		return false;
	}
}

EXPORT MStatus initializePlugin(MObject obj)
{
	MStatus result = MS::kSuccess;

	cout.rdbuf(cerr.rdbuf());
	int msgSize = (1 << 20);
	msg = new char[msgSize / 4];
	MGlobal::displayInfo("buffertest: ");

	CB = new CircleBuffer("buffer", msgSize, true, 256);

	MFnPlugin myPlugin(obj, "Maya Plugin", "1.0", "Any", &result);

	MItDag iterator(MItDag::kDepthFirst);
	MObject parent;

	while (iterator.isDone() == false)
	{
		if (iterator.currentItem().hasFn(MFn::kMesh))
		{
			MFnMesh mesh(iterator.currentItem());
			getMeshInfo(mesh);

			MCallbackId meshCreateId = MNodeMessage::addAttributeChangedCallback(iterator.currentItem(), meshCallback, &result);
			callBackStatus(result, meshCreateId);

		}
		iterator.next();
	}
	MGlobal::displayInfo("SuccessFully loaded Maya Modelviewer");

	return MS::kSuccess;
}

EXPORT MStatus uninitializePlugin(MObject obj)
{
	CB->setHeadPosition(0);
	CB->setTailPosition(0);
	delete CB;
	delete[] msg;
	
	//MFnPlugin plugin(obj);

	MGlobal::displayInfo("SuccessFully unloaded Maya Modelviewer");

	return MS::kSuccess;
}
