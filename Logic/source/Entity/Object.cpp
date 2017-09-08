#include "Entity/Object.h"

using namespace Logic;

Object::Object()
{
}

Object::~Object()
{
}

Graphics::RenderInfo* Logic::Object::getRenderInfo()
{
	return &m_renderInfo;
}
