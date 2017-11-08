#include <Misc\GUI\Sprite.h>
#include <Engine\Constants.h>

Sprite::Sprite(float x, float y, float width, float height, Resources::Textures::Files texture, FloatRect uvs, float alpha)
{
    sprite.alpha = alpha;
    sprite.texture = texture;
    sprite.textureRect = uvs;

    sprite.screenRect = FloatRect(x / WIN_WIDTH, y / WIN_HEIGHT, width / WIN_WIDTH, height / WIN_HEIGHT);

}

Sprite::Sprite(Points screenpoint, Points spritepoint, float x, float y, float width, float height, Resources::Textures::Files texture, FloatRect uvs, float alpha)
{
    sprite.alpha = alpha;
    sprite.texture = texture;
    sprite.textureRect = uvs;


    switch (screenpoint)
    {
    case TOP_LEFT:
        break;
    case BOTTOM_LEFT:
        y += WIN_HEIGHT;
        break;

    case BOTTOM_RIGHT:
        y += WIN_HEIGHT;
        x += WIN_WIDTH;
        break;
    case TOP_RIGHT:
        x += WIN_WIDTH;
        break;
    case CENTER:
        y += WIN_HEIGHT * 0.5f;
        x += WIN_WIDTH * 0.5f;
        break;

    }

    switch (spritepoint)
    {
    case TOP_LEFT:
        break;
    case BOTTOM_LEFT:
        y -= height;
        break;

    case BOTTOM_RIGHT:
        y -= height;
        x -= width;
        break;
    case TOP_RIGHT:
        x -= width;
        break;
    case CENTER:
        y -= height * 0.5f;
        x -= width * 0.5f;
        break;

    }
    sprite.screenRect = FloatRect(x / WIN_WIDTH, y / WIN_HEIGHT, width / WIN_WIDTH, height / WIN_HEIGHT);
}

Sprite::~Sprite()
{
}

//moves the entire texture rectangle by the uv amount specified
void Sprite::moveTexturePos(float u, float v)
{
    sprite.textureRect.topLeft.x += u;
    sprite.textureRect.topLeft.y += v;

    sprite.textureRect.bottomRight.x += u;
    sprite.textureRect.bottomRight.y += v;
}

//moves the corners of the texture rectangle by the uv amount specified
void Sprite::moveTexturePos(float topU, float topV, float bottomU, float bottomV)
{
    sprite.textureRect.topLeft.x += topU;
    sprite.textureRect.topLeft.y += topV;

    sprite.textureRect.bottomRight.x += bottomU;
    sprite.textureRect.bottomRight.y += bottomV;
}

//moves the entire scren position rectangle by the pixel amount specified
void Sprite::moveScreenPos(float x, float y)
{
    sprite.screenRect.topLeft.x += (x / WIN_WIDTH);
    sprite.screenRect.topLeft.y += (y / WIN_HEIGHT);

    sprite.screenRect.bottomRight.x += (x / WIN_WIDTH);
    sprite.screenRect.bottomRight.y += (y / WIN_HEIGHT);
}

//moves the corners of the cren position rectangle rectangle by the pixel amount specified
void Sprite::moveScreenPos(float topX, float topY, float bottomX, float bottomY)
{
    sprite.screenRect.topLeft.x += (topX / WIN_WIDTH);
    sprite.screenRect.topLeft.y += (topY / WIN_HEIGHT);

    sprite.screenRect.bottomRight.x += (bottomX / WIN_WIDTH);
    sprite.screenRect.bottomRight.y += (bottomY / WIN_HEIGHT);
}

void Sprite::setTexturePos(float topU, float topV, float bottomU, float bottomV)
{
    sprite.textureRect.topLeft.x = topU;
    sprite.textureRect.topLeft.y = topV;

    sprite.textureRect.bottomRight.x = bottomU;
    sprite.textureRect.bottomRight.y = bottomV;
}

void Sprite::setScreenPos(Points screenpoint, Points spritepoint, float x, float y, float width, float height)
{
    switch (screenpoint)
    {
    case TOP_LEFT:
        break;
    case BOTTOM_LEFT:
        y += WIN_HEIGHT;
        break;

    case BOTTOM_RIGHT:
        y += WIN_HEIGHT;
        x += WIN_WIDTH;
        break;
    case TOP_RIGHT:
        x += WIN_WIDTH;
        break;
    case CENTER:
        y += WIN_HEIGHT * 0.5f;
        x += WIN_WIDTH * 0.5f;
        break;

    }

    switch (spritepoint)
    {
    case TOP_LEFT:
        break;
    case BOTTOM_LEFT:
        y -= height;
        break;

    case BOTTOM_RIGHT:
        y -= height;
        x -= width;
        break;
    case TOP_RIGHT:
        x -= width;
        break;
    case CENTER:
        y -= height * 0.5f;
        x -= width * 0.5f;
        break;

    }
    sprite.screenRect = FloatRect(x / WIN_WIDTH, y / WIN_HEIGHT, width / WIN_WIDTH, height / WIN_HEIGHT);
}

FloatRect Sprite::getScreenRect() const
{
    FloatRect temp = sprite.screenRect;

    temp.topLeft.x *= WIN_WIDTH;
    temp.topLeft.y *= WIN_HEIGHT;
    temp.bottomRight.x *= WIN_WIDTH;
    temp.bottomRight.y *= WIN_HEIGHT;

    return temp;
}

FloatRect Sprite::getTextureRect() const
{
    return sprite.textureRect;
}

void Sprite::render() const
{
    RenderQueue::get().queue(&sprite);
}
