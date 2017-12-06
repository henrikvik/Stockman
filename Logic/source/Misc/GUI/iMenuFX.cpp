#include <Misc\GUI\iMenuFX.h>
#include <Graphics\include\RenderQueue.h>
#include <Graphics\include\Structs.h>
#include <Misc\RandomGenerator.h>
#include <Misc\Sound\NoiseMachine.h>

using namespace Logic;

/********************************
      Simple Particle Class.    *
*********************************/
Pixel::Pixel(float inX, float inY)
{
    remove = false;

    posX = inX;
    posY = inY;

    gravity = 0.f;
    age = 0.f;
    lifeTime = RandomGenerator::singleton().getRandomFloat(50.f, 300.f);
    speed = RandomGenerator::singleton().getRandomFloat(0.000200f, 0.000400f);

    direction = ((rand() % 360) + 1) * 3.14 / 180.f;

    float size = RandomGenerator::singleton().getRandomFloat(4.f, 8.f);
    renderInfo.screenRect = FloatRect(posX, posY, size / WIN_WIDTH, size / WIN_HEIGHT);

    renderInfo.alpha = 1.f;
    renderInfo.isMoveable = true;
    renderInfo.texture = Resources::Textures::particle;
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

/********************************
    Dust Effect on the book.    *
*********************************/
iMenuFX_Dust::iMenuFX_Dust() { }

iMenuFX_Dust::~iMenuFX_Dust()
{
    m_pixels.clear();
}

void iMenuFX_Dust::press(float x, float y)
{
    Sound::NoiseMachine::Get().playSFX(Sound::UI_SAND_FX, nullptr, true);

    for (int i = 8; --i;)
        m_pixels.push_back(Pixel(x / WIN_WIDTH, y / WIN_HEIGHT));
}

void iMenuFX_Dust::update(float deltaTime)
{
    for (size_t i = 0; i < m_pixels.size(); i++)
    {
        m_pixels[i].update(deltaTime);
        if (m_pixels[i].remove)
            m_pixels.erase(m_pixels.begin() + i);
    }
}

void iMenuFX_Dust::render() const
{
    for (const Pixel& p : m_pixels)
        p.render();
}