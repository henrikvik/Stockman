#ifndef FPS_RENDERER_H
#define FPS_RENDERER_H

#include <Graphics\include\Renderer.h>

namespace Logic
{
	class FPSRenderer
	{
	private:
		int fps, frames;
		float fpsTimer;
        bool isActive;
		Graphics::TextString fpsString;
	public:
		FPSRenderer();
		~FPSRenderer();

		void renderFPS(Graphics::Renderer& renderer);
		void updateFPS(float deltaTime);
	};
}

#endif