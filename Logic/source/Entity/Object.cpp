#include "Entity/Object.h"

using namespace Logic;

Object::Object() 
{
	m_renderInfo.render = true;
	m_renderInfo.meshId = Graphics::ModelID::CUBE;
	m_renderInfo.materialId = 0;
}

Object::~Object() { }

void Object::setWorldTranslation(DirectX::SimpleMath::Matrix translation)
{
	m_renderInfo.translation = translation;
}

void Object::render(Graphics::Renderer& renderer)
{
	if (m_renderInfo.render)
		renderer.queueRender(&m_renderInfo);
}