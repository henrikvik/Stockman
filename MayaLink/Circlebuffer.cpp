#include "Circlebuffer.h"

size_t CircleBuffer::round_Up(size_t value, size_t multiple)
{
	assert(multiple);

	return((value + multiple - 1) / multiple)*multiple;
}

CircleBuffer::CircleBuffer(
	LPCSTR buffName,
	const size_t & buffSize,
	const bool & isProducer,
	const size_t & chunkSize)
{
	mutex = CreateMutex(nullptr, false, "Name");

	this->fileMap = NULL;
	this->fileControl = NULL;

	this->buff_Name = buffName;
	this->buff_size = buffSize;
	this->isProducer = isProducer;
	this->chnk_size = chunkSize;

	this->client_Pos = 0;
	this->msg_Count = 0;

	LPCSTR c = "Control";
	std::string temp = std::string(buff_Name) + c;
	control_Name = temp.c_str();

	createMaps(buff_Name, control_Name);
	updateControlBuffer();

}

CircleBuffer::~CircleBuffer()
{
	UnmapViewOfFile(messageData);
	CloseHandle(fileMap);

	UnmapViewOfFile(controlPointer);
	CloseHandle(fileControl);
}

void CircleBuffer::createMaps(LPCSTR buffName, LPCSTR controlName)
{

#pragma region FileMapregion
	this->fileMap = CreateFileMapping
	(
		INVALID_HANDLE_VALUE,
		NULL, PAGE_READWRITE,
		0,
		(DWORD)buff_size,
		buffName
	);

	if (fileMap == NULL)
		throw std::runtime_error("Could not create filemap");

	this->messageData = (char*)MapViewOfFile(this->fileMap, FILE_MAP_ALL_ACCESS, 0, 0, buff_size);

	if (messageData == NULL)
	{
		CloseHandle(this->fileMap);
		throw std::runtime_error("Could not create map view of file");
	}
#pragma endregion  Creates the map that will handle messages

#pragma region ControlMapregion
	// map for control
	this->fileControl = CreateFileMapping
	(
		INVALID_HANDLE_VALUE, 
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(ControlHeader), 
		controlName
	);

	if (this->fileControl == NULL)
	{
		throw std::runtime_error("Could not create map for control");
	}

	this->controlPointer = (size_t *)MapViewOfFile(this->fileControl, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(ControlHeader));

	if (this->controlPointer == NULL)
	{
		CloseHandle(this->fileControl);
		throw std::runtime_error("Could not create map view of file, control");
	}
#pragma endregion Creates the map that will be used for control

}

void CircleBuffer::updateControlBuffer()
{
	if (isProducer == false)
	{
		controlPointer[HEAD] = 0;
		controlPointer[TAIL] = 0;
		controlPointer[CLIENTC] += 1;
	}
	else
	{
		controlPointer[HEAD] = 0;
	}
}

size_t CircleBuffer::calcFreeMemory()
{
	size_t freeMem;
	if (controlPointer[HEAD] >= controlPointer[TAIL])
		return freeMem = (buff_size - controlPointer[HEAD]) + controlPointer[TAIL];

	else if (controlPointer[HEAD] < controlPointer[TAIL])
		return freeMem = controlPointer[TAIL] - controlPointer[HEAD];
}

bool CircleBuffer::push(const void * msg, size_t length)
{
	client_Pos = controlPointer[HEAD];
	size_t padding = chnk_size - ((client_Pos + sizeof(FileHeader) + length % chnk_size)); 

	// Header isnt alowed to reach the tail. but tail can reach head
	if (calcFreeMemory() > (sizeof(FileHeader) + LENGTHPADDED))
	{
		if (client_Pos == buff_size)
			client_Pos = 0;
		msg_Count += 1;
		FileHeader msgHeader;
		msgHeader.id = msg_Count;
		msgHeader.padding = padding;
		msgHeader.clients = controlPointer[CLIENTC];
		msgHeader.length = length;

		memcpy(&messageData[client_Pos], &msgHeader, sizeof(FileHeader));
		client_Pos += sizeof(FileHeader);
		
		// 
		if (buff_size - client_Pos >= LENGTHPADDED)
		{
			memcpy(&messageData[client_Pos], msg, length);
			client_Pos += length;
			client_Pos += msgHeader.padding;
		}
		else
		{
			size_t wait = buff_size - client_Pos;
			memcpy(&messageData[client_Pos], msg, wait);

			client_Pos = 0;
			size_t remain = length - wait;
			memcpy(&messageData[client_Pos], (char*)msg + wait, remain);
			client_Pos = remain;
		}

		controlPointer[HEAD] = client_Pos;
		return true;
	}
	else
		return false;
}

bool CircleBuffer::pop(char * msg, size_t & lenght)
{
	if (this->client_Pos == controlPointer[HEAD])
		return false; // read all

	WaitForSingleObject(mutex, INFINITE);

	if (client_Pos == buff_size)
		client_Pos = 0;

	// read and move forward
	FileHeader * msgHeader = (FileHeader*)&messageData[this->client_Pos];
	client_Pos += sizeof(FileHeader); 
	
	if (buff_size - client_Pos >= (msgHeader->length + msgHeader->padding))
	{
		memcpy(msg, &messageData[this->client_Pos], msgHeader->length);
		client_Pos += msgHeader->length;
		client_Pos += msgHeader->padding;
	}
	else
	{
		size_t wait = buff_size - client_Pos;
		memcpy(msg, &messageData[this->client_Pos], wait);
		client_Pos = 0;
		size_t remain = msgHeader->length - wait;
		memcpy(msg, &messageData[this->client_Pos], remain);
		client_Pos = remain;
	}

	msgHeader->clients -= 1;

	if (msgHeader->clients = 0)
		controlPointer[TAIL] = client_Pos;

	ReleaseMutex(mutex);


	return true;
		
}

void CircleBuffer::setHeadPosition(size_t position)
{
	controlPointer[HEAD] = position;
}

void CircleBuffer::setTailPosition(size_t position)
{
	controlPointer[TAIL] = position;
}