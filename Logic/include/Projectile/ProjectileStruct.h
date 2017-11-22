#ifndef PROJECTILESTRUCT_H
#define PROJECTILESTRUCT_H

#include <Entity\Entity.h>

#include <Graphics\include\RenderInfo.h>
#include <Graphics\include\Particles\ParticleSystem.h>

namespace Logic
{
	enum ProjectileType
	{
		ProjectileTypeNormal,
        ProjectileTypeCrossbowFire,
		ProjectileTypeGrappling,
		ProjectileTypeMelee,
        ProjectileTypeMeleeParry,
		ProjectileTypeShield,
		ProjectileTypeBulletTime,
		ProjectileTypeBulletTimeSensor,
		ProjectileTypeIce,
        ProjectileTypeFreezeGrenade,
        ProjectileTypeIceShard,
        ProjectileTypeFireArrow
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

        bool hasEffect;
        bool effectVelocity;
        bool effectActivated;
        Graphics::ParticleEffect effect;

		bool isSensor;				// If bullet is sensor or not (collision with other bullet)
		bool enemyBullet;			// if enemies shot it or a player
        bool shouldRender;          // If projectile should render

        ProjectileData() : hasEffect(false), effectActivated(false), effectVelocity(false), effect({}), damage(1), scale(1.f), mass(1.f), speed(1.f), gravityModifier(0.f), ttl(1000), meshID(Resources::Models::UnitCube), materialID(1), type(ProjectileTypeNormal), isSensor(false), enemyBullet(false) {}
		ProjectileData(const char *fx, bool effectVelocity, bool effectActivated, int inDamage, float inScale, float inMass, float inSpeed, float inGravityModifier, float inTTL, Resources::Models::Files inMeshID, int inMaterialID, ProjectileType inType = ProjectileTypeNormal, bool inIsSensor = false, bool inEnemyBullet = false, bool inShouldRender = true) : damage(inDamage), scale(inScale), mass(inMass), speed(inSpeed),
			effectVelocity(effectVelocity), effectActivated(effectActivated), gravityModifier(inGravityModifier), ttl(inTTL), meshID(inMeshID), materialID(inMaterialID), type(inType), isSensor(inIsSensor), enemyBullet(inEnemyBullet), shouldRender(inShouldRender)
        {
            if (fx) {
                hasEffect = true;
                effect = Graphics::FXSystem->getEffect(fx);
            }
            else {
                hasEffect = false;
            }
        }
	};
}

#endif // !PROJECTILESTRUCT_H
