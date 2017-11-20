#include <AI\EnemyNecromancer.h>
#include <AI\Behavior\RangedBehavior.h>
#include <Misc\RandomGenerator.h>
#include <Projectile\Projectile.h>
#include <Misc\ComboMachine.h>

using namespace Logic;

const int EnemyNecromancer::SPEED_AB1 = 15,
          EnemyNecromancer::SPEED_AB2 = 20,
          EnemyNecromancer::MAX_SPAWNED_MINIONS = 4,
          EnemyNecromancer::BASE_DAMAGE = 1,
          EnemyNecromancer::MAX_HP = 50,
          EnemyNecromancer::SCORE = 5;
const float EnemyNecromancer::BASE_SPEED = 7.5f;

EnemyNecromancer::EnemyNecromancer(btRigidBody* body, btVector3 halfExtent)
    : Enemy(Resources::Models::UnitCube, body, halfExtent, MAX_HP, BASE_DAMAGE,
        BASE_SPEED, EnemyType::NECROMANCER, 0) {
    setBehavior(RANGED);
    addCallback(ON_DEATH, [&](CallbackData data) -> void {
        ComboMachine::Get().kill(SCORE);
        RandomGenerator::singleton().getRandomInt(0, 1) ? 
            SpawnTrigger(2, getPositionBT() - btVector3(0.f, data.caller->getRigidBody()->getCollisionShape()->getLocalScaling().y() * 1.5f, 0.f), std::vector<int>{ StatusManager::AMMO_PICK_UP_PRIMARY }) : 
            SpawnTrigger(3, getPositionBT() - btVector3(0.f, data.caller->getRigidBody()->getCollisionShape()->getLocalScaling().y() * 1.5f, 0.f), std::vector<int>{ StatusManager::AMMO_PICK_UP_SECONDARY });
    });
    m_spawnedMinions = 0;

    createAbilities();
}

EnemyNecromancer::~EnemyNecromancer()
{
}

void EnemyNecromancer::createAbilities()
{
    AbilityData data;
    data.cooldown = 800.f;
    data.duration = 0.f;
    data.randomChanche = 0;

    static Graphics::ParticleEffect necroTrail = Graphics::FXSystem->getEffect("NecroProjTrail");
    ProjectileData pdata;
    pdata.effect = necroTrail;
    pdata.hasEffect = true;
    pdata.effectVelocity = false;


    auto onTick = [&](Player &player, Ability &ab) -> void {};

    auto onUse1 = [=](Player &player, Ability &ab) -> void {
        Projectile *pj = shoot(((player.getPositionBT() - getPositionBT()) + btVector3{ 0, 80, 0 }).normalize(), pdata, (float)SPEED_AB2, 2.5f, 0.6f);
		if (pj)
		{
			pj->addCallback(ON_COLLISION, [&](CallbackData &data) -> void {
				if (m_spawnedMinions < MAX_SPAWNED_MINIONS)
				{
					Enemy *e = SpawnEnemy(EnemyType::NECROMANCER_MINION, data.caller->getPositionBT(), {});
					if (e)
					{
						m_spawnedMinions++;

						increaseCallbackEntities();
						e->addCallback(ON_DEATH, [&](CallbackData &data) -> void {
                            ComboMachine::Get().kill();
							m_spawnedMinions--;
						});
						e->addCallback(ON_DESTROY, [&](CallbackData data) -> void {
							decreaseCallbackEntities();
						});
					}
				}
			});
		}
    };

    ab1 = Ability(data, onTick, onUse1);
    
    data.cooldown = 700.f;
    data.randomChanche = 7;
    auto onUse2 = [&](Player &player, Ability &ab) -> void {
        shoot((player.getPositionBT() - getPositionBT()).normalize(), Resources::Models::UnitCube, (float)SPEED_AB1, 0.02f, 0.2f);
    };

    ab2 = Ability(data, onTick, onUse2);
}

void EnemyNecromancer::clear()
{
}

void EnemyNecromancer::onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier)
{
    if (Enemy *e = dynamic_cast<Enemy*>(&other))
    {
    }
    else if (Projectile *pj = dynamic_cast<Projectile*> (&other))
    {
        if (!pj->getProjectileData().enemyBullet)
        {
            damage(static_cast<int> (pj->getProjectileData().damage * dmgMultiplier));

            if (pj->getProjectileData().type == ProjectileTypeBulletTimeSensor)
                getStatusManager().addStatusResetDuration(StatusManager::EFFECT_ID::BULLET_TIME, pj->getStatusManager().getStacksOfEffectFlag(Effect::EFFECT_FLAG::EFFECT_BULLET_TIME));
        }
    }
}

void EnemyNecromancer::onCollision(Player &other)
{

}

void EnemyNecromancer::updateSpecific(Player &player, float deltaTime)
{
    ab1.update(deltaTime, player);
    ab2.update(deltaTime, player);
}

void EnemyNecromancer::updateDead(float deltaTime)
{
}

void EnemyNecromancer::useAbility(Player &target)
{
    ab1.useAbility(target);
    ab2.useAbility(target);
}
