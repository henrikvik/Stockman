#ifndef OBJECT_H
#define OBJECT_H

#include <Graphics\include\Renderer.h>
#include <Graphics\include\Structs.h>
#include <SimpleMath.h>

namespace Logic
{
	class Object
	{
	public:
		Object();
		Object(const Object& other) = delete;
		Object* operator=(const Object& other) = delete;
		virtual ~Object();

		void setWorldTranslation(DirectX::SimpleMath::Matrix translation);
		virtual void render(Graphics::Renderer& renderer);

	private:
		Graphics::RenderInfo m_renderInfo;
	};
}


#endif // !OBJECT_H
