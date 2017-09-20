#ifndef PROJECTILESTRUCT_H
#define PROJECTILESTRUCT_H

#include <Graphics\include\Structs.h>

namespace Logic
{
	struct ProjectileData
	{
		float damage;				// Projectile Damage
		float scale;				// Scale of the projectile
		float mass;					// Mass of projectile
		float speed;				// Bullet speed
		float gravityModifier;		// How fast the bullet falls to the ground
		float ttl;					// Time to live in milisec

		Graphics::ModelID meshID;
		int materialID;

		ProjectileData() : damage(1.f), scale(1.f), mass(1.f), speed(1.f), gravityModifier(0.f), ttl(1000), meshID(Graphics::ModelID::CUBE), materialID(1) {}
		ProjectileData(float inDamage, float inScale, float inMass, float inSpeed, float inGravityModifier, float inTTL, Graphics::ModelID inMeshID, int inMaterialID) : damage(inDamage), scale(inScale), mass(inMass), speed(inSpeed), gravityModifier(inGravityModifier), ttl(inTTL), meshID(inMeshID), materialID(inMaterialID) {}
	};
}

#endif // !PROJECTILESTRUCT_H
