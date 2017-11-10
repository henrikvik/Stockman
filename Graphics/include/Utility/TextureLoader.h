#pragma once
#include <Resources\Resources.h>
#include <unordered_map>
#include "Texture.h"

class TextureLoader
{
public:
    static TextureLoader& get();

    void loadAll();
    void unloadAll();


    Texture * getTexture(Resources::Textures::Files texture);

private:
    TextureLoader();
    virtual ~TextureLoader();

    std::unordered_map<Resources::Textures::Files, Texture*> textures;
};