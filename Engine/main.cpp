#include <Graphics.h>
#include "Engine.h"
#include <Windows.h>
#include <crtdbg.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Engine engine(hInstance, 1280, 720);
	return engine.run();
}