#ifndef iMENUFX_H
#define iMENUFX_H

#include <vector>
#include <Graphics\include\RenderInfo.h>

namespace Logic
{
    class Pixel
    {
    public:
        Pixel(float inX, float inY);

        void update(float dt);
        void render() const;

        float posX, posY, speed, direction, age, lifeTime, gravity;
        bool remove;
        SpriteRenderInfo renderInfo;
    };

    class iMenuFX
    {
    public:
        virtual void press(float x, float y) = 0;
        virtual void update(float dt) = 0;
        virtual void render() const = 0;
    };

    class iMenuFX_Dust : public iMenuFX
    {
    public:
        iMenuFX_Dust();
        ~iMenuFX_Dust();

        void press(float x, float y);
        void update(float dt);
        void render() const;

    private:
        std::vector<Pixel> m_pixels;
    };
}

#endif // !iMENUFX_H
