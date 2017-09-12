#ifndef OBJECT_H
#define OBJECT_H

#include <Graphics\include\Structs.h>

namespace Logic
{
	class Object
	{
	public:
		Object();
		Object(const Object& other) = delete;
		Object* operator=(const Object& other) = delete;
		~Object();

		Graphics::RenderInfo* getRenderInfo();
	private:
		Graphics::RenderInfo m_renderInfo;
	};
}


#endif // !OBJECT_H
