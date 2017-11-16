#ifndef PROJECTILESTRUCT_H
#define PROJECTILESTRUCT_H

#include <Entity\Entity.h>
#include <Graphics\include\RenderInfo.h>

namespace Logic
{
	enum ProjectileType
	{
		ProjectileTypeNormal,
		ProjectileTypeGrappling,
		ProjectileTypeMelee,
        ProjectileTypeMeleeParry,
		ProjectileTypeShield,
		ProjectileTypeBulletTime,
		ProjectileTypeBulletTimeSensor,
		ProjectileTypeIce,
        ProjectileTypeFreezeGrenade,
        ProjectileTypeIceShard
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
		Resources::Models::Files meshID;
		int materialID;

		bool isSensor;				// If bullet is sensor or not (collision with other bullet)
		bool enemyBullet;			// if enemies shot it or a player
        bool shouldRender;          // If projectile should render

		ProjectileData() : damage(1), scale(1.f), mass(1.f), speed(1.f), gravityModifier(0.f), ttl(1000), meshID(Resources::Models::UnitCube), materialID(1), type(ProjectileTypeNormal), isSensor(false), enemyBullet(false) {}
		ProjectileData(int inDamage, float inScale, float inMass, float inSpeed, float inGravityModifier, float inTTL, Resources::Models::Files inMeshID, int inMaterialID, ProjectileType inType = ProjectileTypeNormal, bool inIsSensor = false, bool inEnemyBullet = false, bool inShouldRender = true) : damage(inDamage), scale(inScale), mass(inMass), speed(inSpeed),
			gravityModifier(inGravityModifier), ttl(inTTL), meshID(inMeshID), materialID(inMaterialID), type(inType), isSensor(inIsSensor), enemyBullet(inEnemyBullet), shouldRender(inShouldRender) {}
	};
}

#endif // !PROJECTILESTRUCT_H
