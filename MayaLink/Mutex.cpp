#include "Mutex.h"

Mutex::Mutex()
{
	mutex = CreateMutex(nullptr, false, "uName");
	ms = INFINITE;
}

Mutex::~Mutex()
{

}

void Mutex::lock()
{
	WaitForSingleObject(mutex, ms);
}

void Mutex::unlock()
{
	ReleaseMutex(mutex);
}