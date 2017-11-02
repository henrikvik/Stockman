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
		ProjectileTypeBulletTimeSensor,
		ProjectileTypeIce
	};

	struct ProjectileData
	{
		int damage;				// Projectile Damage
		float scale;				// Scale of the projectile
		float mass;					// Mass of projectile
		float speed;				// Bullet speed
		float gravityModifier;		// How fast the bullet falls to the ground
		float ttl;					// Time to live in milisec

		ProjectileType type;
		Graphics::ModelID meshID;
		int materialID;

		bool isSensor;				// If bullet is sensor or not (collision with other bullet)
		bool enemyBullet;			// if enemies shot it or a player
        bool shouldRender;          // If projectile should render

		ProjectileData() : damage(1), scale(1.f), mass(1.f), speed(1.f), gravityModifier(0.f), ttl(1000), meshID(Graphics::ModelID::CUBE), materialID(1), type(ProjectileTypeNormal), isSensor(false), enemyBullet(false) {}
		ProjectileData(int inDamage, float inScale, float inMass, float inSpeed, float inGravityModifier, float inTTL, Graphics::ModelID inMeshID, int inMaterialID, ProjectileType inType = ProjectileTypeNormal, bool inIsSensor = false, bool inEnemyBullet = false, bool inShouldRender = true) : damage(inDamage), scale(inScale), mass(inMass), speed(inSpeed),
			gravityModifier(inGravityModifier), ttl(inTTL), meshID(inMeshID), materialID(inMaterialID), type(inType), isSensor(inIsSensor), enemyBullet(inEnemyBullet), shouldRender(inShouldRender) {}
	};
}

#endif // !PROJECTILESTRUCT_H
