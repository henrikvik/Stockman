#include "Misc\RenderRegister.h"

using namespace Logic;

RenderRegister::RenderRegister()
{
}


RenderRegister::~RenderRegister()
{
}

std::vector<Graphics::RenderInfo*>* RenderRegister::getRenderInfo()
{
	return &m_renderInfoList;
}

void Logic::RenderRegister::addRenderInfo(Graphics::RenderInfo* renderInfo)
{
	m_renderInfoList.push_back(renderInfo);
}

void Logic::RenderRegister::clear()
{
	m_renderInfoList.clear();
}
