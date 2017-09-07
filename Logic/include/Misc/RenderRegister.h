#ifndef RENDERREGISTER_H
#define RENDERREGISTER_H

#include <vector>
#include "..\..\..\Graphics\include\Structs.h"

namespace Logic
{
	class RenderRegister
	{
	public:
		RenderRegister();
		~RenderRegister();

		std::vector<Graphics::RenderInfo*>* getRenderInfo();
		void addRenderInfo(Graphics::RenderInfo* renderInfo);
		void clear();
	private:
		std::vector<Graphics::RenderInfo*> m_renderInfoList;
	};
}

#endif // !RENDERREGISTER_H


