#ifndef SLIDER_H
#define SLIDER_H

#pragma region ClassDesc
/*
CLASS: Slider
AUTHOR:

DESCRIPTION: Handles input-data for any value in a range
*/
#pragma endregion Description of class

#include <Graphics\include\RenderQueue.h>

#include "Mouse.h"
#include <functional>
#include <Graphics\include\Structs.h>

namespace Logic
{
    class Slider
    {
    public:
        enum State { ACTIVE, INACTIVE, HOVER };

        Slider(
            float x, float y,
            float width, float height,
            Resources::Textures::Files texture,
            FloatRect inactive,
            FloatRect active,
            FloatRect hover,
            float min,
            float max,
            float* value, 
            float minValue,
            float maxValue,
            float delimiter

        );
        ~Slider();

        void updateOnPress(int posX, int posY);
        void updateOnRelease(int posX, int posY);
        void hoverOver(int posX, int posY);
        bool animationTransition(float dt, float maxAnimationTime, bool forward);

        void setState(State state);
        void setAlpha(float alpha);
        void setUVS(FloatRect newUVs);

        void render() const;

    private:
        SpriteRenderInfo renderInfo;

        FloatRect inactive;
        FloatRect active;
        FloatRect hover;
        State state;
        float m_y, m_height, m_width, m_min, m_max, m_minValue, m_maxValue, m_delimiter, m_tempValue;

        float* m_value;

        DirectX::SimpleMath::Vector2 m_animationEnd;
        DirectX::SimpleMath::Vector2 m_animationStart;

        int m_activeOffset;
        bool m_highlighted;
        float m_animationTime;
    };
}
#pragma endregion

#endif