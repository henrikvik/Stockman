#pragma once

#ifdef DLL_EXPORT  
#define LOGIC_API __declspec(dllexport)   
#else  
#define LOGIC_API __declspec(dllimport)   
#endif  

namespace Logic
{
	
}