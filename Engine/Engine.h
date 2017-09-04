#pragma once

#include <Windows.h>


class Engine
{
public:
	Engine(HINSTANCE hInstance, int width, int height);
	virtual ~Engine();

	int run();

private:
	HWND window;
	int width;
	int height;
	HINSTANCE hInstance;

	void initializeWindow();
};