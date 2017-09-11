#include "../Entity/Hitbox.h"

using namespace Logic;

Hitbox::Hitbox() { }

Hitbox::~Hitbox() { }

bool Hitbox::init(Physics* physics, BodyDesc bodyDesc)
{
	this->createBody(physics, bodyDesc);
}