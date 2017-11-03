#pragma once
#include <Graphics\include\RenderQueue.h>


class Sprite
{
public:
    enum Points
    {
        TOP_LEFT,
        TOP_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_RIGHT,
        CENTER
        
    };
    Sprite(float x, float y, float width, float height, Resources::Textures::Files texture, FloatRect uvs, float alpha = 1);
    Sprite(Points screenpoint, Points spritepoint, float x, float y, float width, float height, Resources::Textures::Files texture, FloatRect uvs, float alpha = 1);
    ~Sprite();

    void render() const;
private:
    SpriteRenderInfo sprite;

};
