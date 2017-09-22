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
		virtual ~Object();

		void setWorldTranslation(DirectX::SimpleMath::Matrix translation);
		virtual void render(Graphics::Renderer& renderer);

	private:
		Graphics::RenderInfo m_renderInfo;
	};
}


#endif // !OBJECT_H
