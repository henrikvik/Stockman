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

    void moveTexturePos(float u, float v);
    void moveTexturePos(float topU, float topV, float bottomU, float bottomV);
    void moveScreenPos(float x, float y);
    void moveScreenPos(float topX, float topY, float bottomX, float bottomY);
    void setTexturePos(float topU, float topV, float bottomU, float bottomV);
    void setScreenPos(Points screenpoint, Points spritepoint, float x, float y, float width, float height);
    void setTopYToProcent(float procent, float yMax, float ymaxPos, float yminPos);
    FloatRect getScreenRect()const;
    FloatRect getTextureRect()const;
    void render() const;
private:
    SpriteRenderInfo sprite;

};
