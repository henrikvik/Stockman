#ifndef OBJECT_H
#define OBJECT_H

#include <d3d11.h>

namespace Logic
{
	class Object
	{
	public:
		Object();
		Object(const Object& other) = delete;
		Object* operator=(const Object& other) = delete;
		~Object();

	private:
		
	};
}


#endif // !OBJECT_H
