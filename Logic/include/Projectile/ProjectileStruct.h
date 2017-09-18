#ifndef PROJECTILESTRUCT_H
#define PROJECTILESTRUCT_H

#include <Graphics\include\Structs.h>

namespace Logic
{
	struct ProjectileData
	{
		float damage;				// Projectile Damage
		float scale;				// Scale of the projectile
		float speed;				// Bullet speed
		float gravityModifier;		// How fast the bullet falls to the ground

		Graphics::ModelID meshID;
		int materialID;

		ProjectileData() : damage(1.f), scale(1.f), speed(1.f), gravityModifier(0.f), meshID(Graphics::ModelID::CUBE), materialID(1) {}
		ProjectileData(float inDamage, float inScale, float inSpeed, float inGravityModifier, Graphics::ModelID inMeshID, int inMaterialID) : damage(inDamage), scale(inScale), speed(inSpeed), gravityModifier(inGravityModifier), meshID(inMeshID), materialID(inMaterialID) {}
	};
}

#endif // !PROJECTILESTRUCT_H
