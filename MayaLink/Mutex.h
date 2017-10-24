#pragma once
#include <Windows.h>

class Mutex
{
private:
	HANDLE mutex;
	DWORD ms;
public:
	Mutex();
	~Mutex();

	void lock();
	void unlock();
};

