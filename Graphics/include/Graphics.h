#pragma once

#include "ThrowIfFailed.h"

#ifdef DLL_EXPORT
#define GRAPHICS_API __declspec(dllexport)   
#else  
#define GRAPHICS_API __declspec(dllimport)   
#endif  

#include "Renderer.h"

namespace Graphics
{
    static GRAPHICS_API Renderer;
}