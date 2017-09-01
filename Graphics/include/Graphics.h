#pragma once

#ifdef DLL_EXPORT
#define GRAPHICS_API __declspec(dllexport)   
#else  
#define GRAPHICS_API __declspec(dllimport)   
#endif  

#include "Renderer.h"

namespace Graphics
{
	
}