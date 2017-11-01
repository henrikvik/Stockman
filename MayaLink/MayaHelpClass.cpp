#pragma once

#include "MayaHelpClass.h"

MayaHelpClass::MayaHelpClass()
{

}

MayaHelpClass::~MayaHelpClass()
{

}

void MayaHelpClass::copy(float dest[], MFloatPoint & src)
{
	dest[0] = src.x;
	dest[1] = src.y;
	dest[2] = src.z;
}

void MayaHelpClass::copy(float dest[], MFloatVector & src)
{
	dest[0] = src.x;
	dest[1] = src.y;
	dest[2] = src.z;
}

void MayaHelpClass::getMeshInfo(MFnMesh & mesh)
{
	mesh.getPoints(points, MSpace::kObject);
	mesh.getUVs(U, V, 0);
	mesh.getAssignedUVs(uvCounts, uvIDS);

	mesh.getTriangleOffsets(triangleCountsOffsets, triangleIndices);
	mesh.getVertices(vertexCount, polyVerticesID);
	mesh.getNormals(normals, MSpace::kObject);

	//	vertices.resize(triangleIndices.length);

	mesh.getNormalIds(normalCount, normalList);

	for (unsigned int i = 0; i < triangleIndices.length(); i++)
	{
		copy(vertices[i].positions, points[polyVerticesID[triangleIndices[i]]]);
		copy(vertices[i].normals, normals[normalList[triangleIndices[i]]]);

		vertices[i].uvs[0] = U[uvIDS[triangleIndices[i]]];
		vertices[i].uvs[1] = V[uvIDS[triangleIndices[i]]];
	}

#pragma region CopyRegion
	offset = 0;

	int Type = MessageType::MMesh;
	memcpy(msg, &Type, sizeof(int));
	offset += sizeof(int);

	memcpy(&meshType, mesh.name().asChar(), sizeof(const char[256]));
	meshType.vertexArray = nullptr;
	meshType.vertexCount = vertices.size();

	memcpy((msg + offset), &meshType, sizeof(MeshType));
	offset += sizeof(MeshType);

	memcpy(msg + offset, vertices.data(), sizeof(Vertices) * vertices.size());
	offset += sizeof(vertices) * vertices.size();

	//circleBuffer->push(msg, offset);
#pragma endregion

}

void MayaHelpClass::getTransform(MFnTransform & transform, MFnMesh &mesh)
{
	transform.getScale(transformType.scale);
	transform.getRotationQuaternion
	(
		transformType.rotation[0],
		transformType.rotation[1],
		transformType.rotation[2],
		transformType.rotation[3]
	);

	transformType.translation[0] = transform.getTranslation(MSpace::kTransform).x;
	transformType.translation[1] = transform.getTranslation(MSpace::kTransform).y;
	transformType.translation[2] = transform.getTranslation(MSpace::kTransform).z;

	offset = 0;

	int type = MessageType::MTransform;
	memcpy(msg, &type, sizeof(type));

	offset += sizeof(type);

	memcpy(&transformType.meshName, mesh.name().asChar(), sizeof(transformType.meshName));
	memcpy((msg + offset), &transformType, sizeof(TransformType));

	offset += sizeof(TransformType);

	//circleBuffer->push(msg, offset);

}

MPlug MayaHelpClass::plugSearch(MFnDependencyNode &node, MString string)
{
	return node.findPlug(string);
}

void MayaHelpClass::getMaterial(MObject &iterator)
{
	MFnDependencyNode materialNode(iterator);

	DisplayI("Material name: " + materialNode.name());

	outColor = plugSearch(materialNode, "Outcolor");
	color = plugSearch(materialNode, "color");
	diffuse = plugSearch(materialNode, "diffuse");

	color.connectedTo(textureGroup, true, false, &result);

	color.getValue(data);
	MFnNumericData nData(data);
	nData.getData
	(
		material.color[0],
		material.color[1],
		material.color[2]
	);
	diffuse.getValue(material.diffuse);
	material.texture = false;

	for (int i = 0; i < textureGroup.length(); i++)
	{
		MString file;
		MFnDependencyNode textureNode(textureGroup[i].node());

		plugSearch(textureNode, "fileTextureName").getValue(file);
		memcpy(&material.textureFilePath, file.asChar(), sizeof(material.textureFilePath));

		material.texture = true;
	}


#pragma region Copyregion
	offset = 0;

	int Type = MessageType::MMaterial;
	memcpy(msg, &Type, sizeof(int));
	offset += sizeof(int);

	memcpy(&material.materialName, materialNode.name().asChar(), sizeof(const char[256]));
	memcpy((msg + offset), &material, sizeof(Material));
	offset += sizeof(Material);

	memcpy(msg + offset, meshVector.data(), sizeof(Meshes) * meshVector.size());
	offset += sizeof(Meshes) * meshVector.size();

	//circleBuffer->push(msg, offset);
	meshVector.clear();
#pragma endregion

}

void MayaHelpClass::surfaceShaderMaterial(MPlug &Plug)
{
	outColor.connectedTo(shadingGroupArr, false, true, &result);

	for (int i = 0; i < shadingGroupArr.length(); i++)
	{
		if (shadingGroupArr[i].node().hasFn(MFn::kShadingEngine))
		{
			MFnDependencyNode shadingNode = shadingGroupArr[i].node();


		}
	}
}

void MayaHelpClass::getCamera()
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
