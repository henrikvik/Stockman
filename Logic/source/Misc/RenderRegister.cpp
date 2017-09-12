#include "Misc\RenderRegister.h"

using namespace Logic;

RenderRegister::RenderRegister() { }

RenderRegister::~RenderRegister() { }

std::queue<Graphics::RenderInfo*>* RenderRegister::getRenderInfo()
{
	return &m_renderInfoList;
}

void RenderRegister::addRenderInfo(Graphics::RenderInfo* renderInfo)
{
	m_renderInfoList.push(renderInfo);
}

void RenderRegister::clear()
{
	std::queue<Graphics::RenderInfo*> empty;
	std::swap(m_renderInfoList, empty);
}
