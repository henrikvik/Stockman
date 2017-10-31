#include <Graphics.h>
#include "Engine.h"
#include <Windows.h>
#include <crtdbg.h>
#include <Game.h>
#include "Constants.h"
#include <Resources\ResourceManager.h>

// int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Engine engine(hInstance, WIN_WIDTH, WIN_HEIGHT);
	return engine.run();
}