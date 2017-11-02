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


#include <Graphics\include\RenderQueue.h>

#include "Mouse.h"
#include <functional>
#include <Graphics\include\Structs.h>


namespace Logic
{
	class Button
	{
	public:
        enum State { ACTIVE, INACTIVE, HOVER };

        Button(Resources::Textures::Files texture, 
            FloatRect screenRect, 
            FloatRect inactive, 
            std::function<void(void)> callback);

        Button(Resources::Textures::Files texture, 
            FloatRect screenRect, 
            FloatRect inactive, 
            FloatRect active, 
            std::function<void(void)> callback);

        Button(Resources::Textures::Files texture, 
            FloatRect screenRect, 
            FloatRect inactive, 
            FloatRect active, 
            FloatRect hover, 
            std::function<void(void)> callback);

		~Button();
        
        void setCallback(std::function<void(void)> callback);

		void updateOnPress(int posX, int posY);
        void hoverOver(int posX, int posY);
		bool animationTransition(float dt, float maxAnimationTime, bool forward);

        void setState(State state);

        void render() const;
	private:
		Graphics::ButtonInfo m_buttonInfo;

        Resources::Textures::Files texture;

        FloatRect screenRect;
        FloatRect inactive;
        FloatRect active;
        FloatRect hover;
		std::function<void(void)> callback;

        FloatRect * textureRect;
        State state;

		DirectX::SimpleMath::Vector2 m_animationEnd;
		DirectX::SimpleMath::Vector2 m_animationStart;

		int m_activeOffset;
        bool m_highlighted;
        float m_animationTime;
	};
}
#endif