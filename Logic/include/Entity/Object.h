#ifndef OBJECT_H
#define OBJECT_H

#include <d3d11.h>
#include <SimpleMath.h>

namespace Graphics 
{
    class Renderer;
    struct RenderInfo;
    enum ModelID;
};

namespace Logic
{
	class Object
	{
	public:
		Object();
		Object(Graphics::ModelID modelID);
        Object(const Object &other) = delete;
        Object* operator=(const Object &other) = delete;
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
		Graphics::RenderInfo* m_renderInfo;
	};
}


#endif // !OBJECT_H
