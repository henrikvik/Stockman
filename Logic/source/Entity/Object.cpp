#include "Entity/Object.h"
#include <Graphics\include\Renderer.h>
#include <Graphics\include\Structs.h>

using namespace Logic;

// Defaults into a cube with material = 0
Object::Object() 
{
    m_renderInfo = new Graphics::RenderInfo();
	m_renderInfo->render = true;
	m_renderInfo->meshId = Graphics::ModelID::CUBE;
	m_renderInfo->materialId = 0;
    renderInfo.model = Resources::Models::UnitCube;
}

// This constructor should be used on release (add materialID here aswell, when implemented)
Object::Object(Graphics::ModelID modelID)
{
    m_renderInfo = new Graphics::RenderInfo();
	m_renderInfo->render = true;
	m_renderInfo->meshId = modelID;
	m_renderInfo->materialId = 0;
    renderInfo.model = Resources::Models::UnitCube;
}

Object::~Object() 
{
    delete m_renderInfo;
}

void Object::render(Graphics::Renderer& renderer)
{
	if (m_renderInfo->render)
		renderer.queueRender(m_renderInfo);
    if (m_renderInfo.render)
    {
        renderer.queueRender(m_renderInfo);
        RenderQueue_t::get().queue(&renderInfo);
    }
}

void Object::setShouldRender(bool render)
{
	m_renderInfo->render = render;
}

void Object::setMaterialID(int id)
{
	m_renderInfo->materialId = id;
}

void Object::setModelID(Graphics::ModelID modelID)
{
	m_renderInfo->meshId = modelID;
}

void Object::setWorldTranslation(DirectX::SimpleMath::Matrix translation)
{
	m_renderInfo->translation = translation;
    renderInfo.transform = translation;
}

int Object::getMaterialID() const
{
	return m_renderInfo->materialId;
}

Graphics::ModelID Object::getModelID() const
{
	return m_renderInfo->meshId;
}

DirectX::SimpleMath::Matrix Object::getWorldTranslation() const
{
	return m_renderInfo->translation;
}

bool Object::getShouldRender() const
{
	return m_renderInfo->render;
}