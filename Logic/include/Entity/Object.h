#ifndef OBJECT_H
#define OBJECT_H

#include <Graphics\include\Structs.h>
#include <Misc\RenderRegister.h>

namespace Logic
{
	class Object
	{
	public:
		Object();
		Object(const Object& other) = delete;
		Object* operator=(const Object& other) = delete;
		~Object();

		virtual void render(RenderRegister& rRegister);

	private:
		Graphics::RenderInfo m_renderInfo;
	};
}


#endif // !OBJECT_H
