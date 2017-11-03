#include <Graphics.h>
#include "Engine.h"
#include <Windows.h>
#include <crtdbg.h>
#include <StateMachine.h>
#include "Constants.h"
#include <Resources\ResourceManager.h>

// int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    int args;
    LPWSTR *cmd = CommandLineToArgvW(GetCommandLineW(), &args);
	Engine engine(hInstance, WIN_WIDTH, WIN_HEIGHT, cmd, args);
    LocalFree(cmd);

	return engine.run();
}