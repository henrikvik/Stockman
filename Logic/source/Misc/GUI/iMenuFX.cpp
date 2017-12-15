#include <Misc\GUI\iMenuFX.h>
#include <Graphics\include\RenderQueue.h>
#include <Graphics\include\Structs.h>
#include <Misc\RandomGenerator.h>
#include <Misc\Sound\NoiseMachine.h>

using namespace Logic;

/********************************
      Simple Particle Class.    *
*********************************/
Pixel::Pixel(float inX, float inY, Resources::Textures::Files texture, float lifeTimeMin, float lifeTimeMax, float sizeMin, float sizeMax)
{
    remove = false;

    posX = inX;
    posY = inY;

    gravity = 0.f;
    age = 0.f;
    lifeTime = RandomGenerator::singleton().getRandomFloat(lifeTimeMin, lifeTimeMax);
    speed = RandomGenerator::singleton().getRandomFloat(0.000200f, 0.000400f);

    direction = ((rand() % 360) + 1) * 3.14 / 180.f;

    float size = RandomGenerator::singleton().getRandomFloat(sizeMin, sizeMax);
    renderInfo.screenRect = FloatRect(posX, posY, size / WIN_WIDTH, size / WIN_HEIGHT);

    renderInfo.alpha = 1.f;
    renderInfo.isMoveable = true;
    renderInfo.texture = texture;
    renderInfo.textureRect = FloatRect(0, 0, 1, 1);
}

void Pixel::update(float dt)
{
    if (remove)
        return;

    age += dt;
    if (age >= lifeTime)
    {
        remove = true;
        renderInfo.alpha = 0.f;
    }
    else
    {
        renderInfo.alpha = 1.f - (age / lifeTime);
    }

    speed += (-speed) * 0.0025f * dt;
    gravity += (0.0015f - gravity) * 0.0015f * dt;

    float velX = speed * sin(direction);
    float velY = speed * cos(direction);
    velY += gravity;

    posX += velX * dt;
    posY += velY * dt;

    renderInfo.screenRect = FloatRect(posX, posY, renderInfo.screenRect.bottomRight.x - renderInfo.screenRect.topLeft.x, renderInfo.screenRect.bottomRight.y - renderInfo.screenRect.topLeft.y);
}

void Pixel::render() const
{
    QueueRender(renderInfo);
}

/*********************************
 Abstract Particle Menu Class    *
*********************************/

void iMenuFX_Particles::update(float dt)
{
    for (size_t i = 0; i < m_pixels.size(); i++)
    {
        m_pixels[i].update(dt);
        if (m_pixels[i].remove)
        {
            std::swap(m_pixels[i], m_pixels[m_pixels.size() - 1]);
            m_pixels.pop_back();
            i--;
        }
    }
}

void iMenuFX_Particles::render() const
{
    for (const Pixel& p : m_pixels)
        p.render();
}

/********************************
    Dust Effect on the book.    *
*********************************/
iMenuFX_Dust::iMenuFX_Dust() { m_type = FX_Dust; }
iMenuFX_Dust::~iMenuFX_Dust() { m_pixels.reserve(0); }

void iMenuFX_Dust::press(float x, float y)
{
    //Sound::NoiseMachine::Get().playSFX(Sound::UI_SAND_FX, nullptr, true);

    for (int i = 8; --i;)
        m_pixels.push_back(Pixel(x / WIN_WIDTH, y / WIN_HEIGHT, Resources::Textures::particle, 50.f, 300.f, 4.f, 8.f));
}

/********************************
    Cute Combo Explosion.       *
*********************************/
iMenuFX_Combo::iMenuFX_Combo() { m_type = FX_Combo; }
iMenuFX_Combo::~iMenuFX_Combo() { m_pixels.reserve(0); }

void iMenuFX_Combo::press(float x, float y)
{
    //Sound::NoiseMachine::Get().playSFX(Sound::UI_SAND_FX, nullptr, true);

    for (int i = 10; --i;)
        m_pixels.push_back(Pixel(x / WIN_WIDTH, y / WIN_HEIGHT, Resources::Textures::particle_white, 50.f, 300.f, 2.f, 7.f));
}

/********************************
    Cute Combo-OVER Explosion.  *
*********************************/
iMenuFX_NewScore::iMenuFX_NewScore() { m_type = FX_NewScore; }
iMenuFX_NewScore::~iMenuFX_NewScore() { m_pixels.reserve(0); }

void iMenuFX_NewScore::press(float x, float y)
{
    //Sound::NoiseMachine::Get().playSFX(Sound::COMBO_DONE, nullptr, true);

    for (int i = 25; --i;)
        m_pixels.push_back(Pixel(x / WIN_WIDTH, y / WIN_HEIGHT, Resources::Textures::particle_white, 50.f, 500.f, 5.f, 12.f));
}