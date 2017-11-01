#ifndef FPS_RENDERER_H
#define FPS_RENDERER_H

#include <Graphics\include\RenderQueue.h>

namespace Logic
{
	class FPSRenderer
	{
	private:
		int fps, frames;
		float fpsTimer;
		TextRenderInfo fpsString;
	public:
		FPSRenderer();
		~FPSRenderer();

		void render();
		void updateFPS(float deltaTime);
	};
}

#endif