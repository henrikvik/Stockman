#pragma once

#include <Windows.h>


class Engine
{
private:
	HWND window;
	int width;
	int height;
	HINSTANCE hInstance;

	void initializeWindow();

public:
	Engine(HINSTANCE hInstance, int width, int height);
	virtual ~Engine();

	int run();
};