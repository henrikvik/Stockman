#pragma once
#include "CircleBuffer.h"
#include <iostream>
#include <vector>

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

struct Vertex
{
	float pos[3];
	float norm[3];
	float UV[2];
};

struct Mesh
{
	char Name[256];
	Vertex* vertexArray; //pointer to allocated vertexArray Memory
	size_t vertexCount;
};

class Importer
{
private:
	Vertex* vertices;
	char * msg;
	CircleBuffer * CB;

public:

	Mesh * mesh;

	bool read();

	int messageType;
	Importer();
	~Importer();
};

