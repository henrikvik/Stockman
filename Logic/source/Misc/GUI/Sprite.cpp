#include <Misc\GUI\Sprite.h>
#include <Engine\Constants.h>

Sprite::Sprite(float x, float y, float width, float height, Resources::Textures::Files texture, FloatRect uvs, float alpha)
{
    this->sprite.alpha = alpha;
    this->sprite.texture = texture;
    this->sprite.textureRect = uvs;

    this->sprite.screenRect = FloatRect(x / WIN_WIDTH, y / WIN_HEIGHT, width / WIN_WIDTH, height / WIN_HEIGHT);

}

Sprite::Sprite(Points screenpoint, Points spritepoint, float x, float y, float width, float height, Resources::Textures::Files texture, FloatRect uvs, float alpha)
{
    this->sprite.alpha = alpha;
    this->sprite.texture = texture;
    this->sprite.textureRect = uvs;


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
    this->sprite.screenRect = FloatRect(x / WIN_WIDTH, y / WIN_HEIGHT, width / WIN_WIDTH, height / WIN_HEIGHT);
}

Sprite::~Sprite()
{
}

void Sprite::render() const
{
    RenderQueue::get().queue(&sprite);
}
