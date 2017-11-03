#include "TextureLoader.h"

TextureLoader & TextureLoader::get()
{
    static TextureLoader loader;
    return loader;
}

void TextureLoader::loadAll()
{
    for (auto & texture : Resources::Textures::Paths)
    {
        textures[texture.first] = new Texture(texture.second);
    }
}

void TextureLoader::unloadAll()
{
    for (auto & texture : textures)
    {
        delete texture.second;
    }
    textures.clear();
}

Texture * TextureLoader::getTexture(Resources::Textures::Files texture)
{
    return textures.at(texture);
}

TextureLoader::TextureLoader()
{
}

TextureLoader::~TextureLoader()
{
    unloadAll();
}
