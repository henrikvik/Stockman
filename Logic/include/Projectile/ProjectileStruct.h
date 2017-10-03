#ifndef PROJECTILESTRUCT_H
#define PROJECTILESTRUCT_H

#include <Graphics\include\Structs.h>
#include <Entity\Entity.h>

namespace Logic
{
	enum ProjectileType
	{
		ProjectileTypeNormal,
		ProjectileTypeGrappling,
		ProjectileTypeMelee,
		ProjectileTypeShield,
		ProjectileTypeBulletTime,
		ProjectileTypeIce
	};

	struct ProjectileData
	{
		float damage;				// Projectile Damage
		float scale;				// Scale of the projectile
		float mass;					// Mass of projectile
		float speed;				// Bullet speed
		float gravityModifier;		// How fast the bullet falls to the ground
		float ttl;					// Time to live in milisec

		bool enemyBullet; // if enemies shot it or a player

		ProjectileType type;
		Graphics::ModelID meshID;
		int materialID;

		ProjectileData() : damage(1.f), scale(1.f), mass(1.f), speed(1.f), gravityModifier(0.f), ttl(1000), meshID(Graphics::ModelID::CUBE), materialID(1), type(ProjectileTypeNormal), enemyBullet(false) {}
		ProjectileData(float inDamage, float inScale, float inMass, float inSpeed, float inGravityModifier, float inTTL, Graphics::ModelID inMeshID, int inMaterialID, ProjectileType inType = ProjectileTypeNormal) : damage(inDamage), scale(inScale), mass(inMass), speed(inSpeed),
			gravityModifier(inGravityModifier), ttl(inTTL), meshID(inMeshID), materialID(inMaterialID), type(inType), enemyBullet(false) {}
	};
}

#endif // !PROJECTILESTRUCT_H
