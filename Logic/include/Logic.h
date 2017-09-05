#ifndef LOGIC_H
#define LOGIC_H

#ifdef DLL_EXPORT  
#define LOGIC_API __declspec(dllexport)   
#else  
#define LOGIC_API __declspec(dllimport)   
#endif  

#include "Game.h"

namespace Logic
{
	static LOGIC_API Game;
}

#endif // !LOGIC_H