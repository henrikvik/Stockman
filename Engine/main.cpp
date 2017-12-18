#define BT_NO_SIMD_OPERATOR_OVERLOADS

#include <Windows.h>
#include "Engine.h"
#include "Constants.h"
#include <iostream>

#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>

// For release
int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    int r = 0;
    try
    {
        int args;
        LPWSTR *cmd = CommandLineToArgvW(GetCommandLineW(), &args);
        Engine engine(hInstance, WIN_WIDTH, WIN_HEIGHT, cmd, args);
        LocalFree(cmd);

        r = engine.run();
    }
    catch (const com_exception& e)
    {
        std::cerr << e.what() << std::endl;
        system("pause");
    }
    
	return r;
}