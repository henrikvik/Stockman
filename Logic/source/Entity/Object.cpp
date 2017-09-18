#include "Entity/Object.h"

using namespace Logic;

Object::Object() 
{
	m_renderInfo.render = true;
}

Object::~Object() { }

void Object::setWorldMatrix(DirectX::SimpleMath::Matrix translation)
{
    m_renderInfo.translation = translation;
}

void Object::render(RenderRegister& rRegister)
{
	if (m_renderInfo.render)
		rRegister.addRenderInfo(&m_renderInfo);
}