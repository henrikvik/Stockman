#ifndef BUTTON_H
#define BUTTON_H

#pragma region Comment

/*
Class: Button
Author: Andreas Eriksson

Description:
This class creates buttons.

*/
#pragma endregion Description of class
#include <d3d11.h>
#include <SimpleMath.h>
#include "Mouse.h"
#include <functional>
#include <Graphics\include\Structs.h>


namespace Logic
{
	class Button
	{
	private:
		Graphics::ButtonInfo m_buttonInfo;

		std::function<void(void)> m_callback;
		DirectX::SimpleMath::Vector2 m_animationEnd;
		DirectX::SimpleMath::Vector2 m_animationStart;

		int m_activeOffset;
        bool m_highlighted;
        float m_animationTime, m_start, m_end;
	public:
		Button();
		~Button();
		void initialize(DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector2 texCoordStart,
			DirectX::SimpleMath::Vector2 texCoordEnd, float offset, float height, float width,
			int textureIndex, std::function<void(void)> callback);
		void updateOnPress(int posX, int posY);
        void hoverOver(int posX, int posY);
		bool animationTransition(float dt, float maxAnimationTime, bool forward);
		Graphics::ButtonInfo* getButtonInfo();
        void setStartAndEnd(float start, float end);
	};
}
#endif