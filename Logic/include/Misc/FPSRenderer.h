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

        TextRenderInfo renderInfo;
	public:
		FPSRenderer();
		~FPSRenderer();

		void updateFPS(float deltaTime);
        void render() const;
	};
}

#endif