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
		/*DirectX::SimpleMath::Rectangle m_rek;
		DirectX::SimpleMath::Vector2 m_texCoordStart;
		DirectX::SimpleMath::Vector2 m_texCoordEnd;
		std::string m_texture;*/
		Graphics::ButtonInfo buttonInfo;
		std::function<void(void)> m_CallBack;
		DirectX::SimpleMath::Vector2 m_animationEnd;
		DirectX::SimpleMath::Vector2 m_animationStart;
		float m_animationTime;
	public:
		Button();
		~Button();
		void initialize(DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector2 texCoordStart, DirectX::SimpleMath::Vector2 texCoordEnd, float height, float width, std::string texture, std::function<void(void)> callBack);
		void updateOnPress(int posX, int posY);
		bool animationTransition(float dt, float maxAnimationTime, bool forward);
		Graphics::ButtonInfo *getButtonInfo();
	};
}
#endif