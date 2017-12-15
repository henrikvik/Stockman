#ifndef iMENUFX_H
#define iMENUFX_H

#include <vector>
#include <Graphics\include\RenderInfo.h>

/* 

    Yes, Im fully aware of this shit class.
    And I got no intention of making it any better.

*/

namespace Logic
{
    class Pixel
    {
    public:
        Pixel(float inX, float inY, Resources::Textures::Files texture, float lifeTimeMin, float lifeTimeMax, float sizeMin, float sizeMax);
        virtual ~Pixel() {  }

        void update(float dt);
        void render() const;

        float posX, posY, speed, direction, age, lifeTime, gravity;
        bool remove;
        SpriteRenderInfo renderInfo;
    };

    enum FX_TYPE
    {
        FX_Dust, FX_Combo, FX_NewScore
    };

    class iMenuFX
    {
    public:
        virtual ~iMenuFX() { }
        virtual void press(float x, float y) = 0;
        virtual void update(float dt) = 0;
        virtual void render() const = 0;
        FX_TYPE getType() { return m_type; }

    protected:
        FX_TYPE m_type;
    };

    class iMenuFX_Particles : public iMenuFX
    {
    public:
        virtual ~iMenuFX_Particles() { m_pixels.clear(); };
        virtual void update(float dt);
        virtual void render() const;

    protected:
        std::vector<Pixel> m_pixels;
    };

    class iMenuFX_Dust : public iMenuFX_Particles
    {
    public:
        iMenuFX_Dust();
        virtual ~iMenuFX_Dust();

        void press(float x, float y);
    };

    class iMenuFX_Combo : public iMenuFX_Particles
    {
    public:
        iMenuFX_Combo();
        virtual ~iMenuFX_Combo();

        void press(float x, float y);
    };

    class iMenuFX_NewScore : public iMenuFX_Particles
    {
    public:
        iMenuFX_NewScore();
        virtual ~iMenuFX_NewScore();

        void press(float x, float y);
    };
}

#endif // !iMENUFX_H
