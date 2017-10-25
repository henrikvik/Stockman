#ifndef OBJECT_H
#define OBJECT_H

#include <Graphics\include\Structs.h>
#include <SimpleMath.h>
#include <Graphics\include\RenderQueue.h>


namespace Graphics
{
	class Renderer;
}

namespace Logic
{
	class Object
	{
	public:
		Object();
		Object(Graphics::ModelID modelID);
		virtual ~Object();
		virtual void render(Graphics::Renderer& renderer);

		void setShouldRender(bool render);
		void setMaterialID(int id);
		void setModelID(Graphics::ModelID modelID);
		void setWorldTranslation(DirectX::SimpleMath::Matrix translation);
		bool getShouldRender() const;
		int getMaterialID() const;
		Graphics::ModelID getModelID() const;
		DirectX::SimpleMath::Matrix getWorldTranslation() const;

	private:
		Graphics::RenderInfo m_renderInfo;
        StaticRenderInfo renderInfo;
	};
}


#endif // !OBJECT_H
