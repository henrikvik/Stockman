#include "../Entity/Hitbox.h"

using namespace Logic;

Hitbox::Hitbox() { }

Hitbox::~Hitbox() { }

bool Hitbox::init(Physics* physics, BodyDesc bodyDesc)
{
	this->createBody(physics, bodyDesc);
}

void Hitbox::clear() { }

void Hitbox::updateSpecific(float deltatTime) { }

void Hitbox::onCollision(Entity & other) { }