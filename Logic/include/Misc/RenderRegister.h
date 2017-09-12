#ifndef RENDERREGISTER_H
#define RENDERREGISTER_H

#pragma region Comment

/*
	Class: RenderRegister
	Author:

	Description:
	This class handles info about all Objects that are being rendered for the current frame.

*/

#pragma endregion Description of class


#include <queue>
#include <Graphics/include/Structs.h>

namespace Logic
{
	class RenderRegister
	{
	public:
		RenderRegister();
		~RenderRegister();

		std::queue<Graphics::RenderInfo*>* getRenderInfo();		//< Returns a list of all renderInfos
		void addRenderInfo(Graphics::RenderInfo* renderInfo);		//< Add renderInfo to list of renderInos
		void clear();
	private:
		std::queue<Graphics::RenderInfo*> m_renderInfoList;		//< List of renderInfos
	};
}

#endif // !RENDERREGISTER_H


