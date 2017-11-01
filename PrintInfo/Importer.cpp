#include "Importer.h"



bool Importer::read()
{
	if (CB->recieve(msg))
	{
		size_t offset = 0;
		memcpy(&messageType, msg, sizeof(messageType));
		offset += sizeof(messageType);

		if (messageType == MessageType::MMesh)
		{
			mesh = (Mesh*)(msg + offset);
			offset += sizeof(Mesh);
			memcpy(this->vertices, (msg + offset), sizeof(Mesh) * mesh->vertexCount);
			mesh->vertexArray = this->vertices;


			return true;
		}
	}
	else
		return false;
}

Importer::Importer()
{
	int msg_Size = (1 << 20) / 4;
	this->msg = new char[msg_Size];
	this->vertices = new Vertex[msg_Size];
	this->CB = new CircleBuffer("buffer", msg_Size, false, 256);
}

Importer::~Importer()
{
	delete[] msg;
	delete[] vertices;
	delete[] CB;
}
