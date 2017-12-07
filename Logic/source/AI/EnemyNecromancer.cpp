#include <AI\EnemyNecromancer.h>
#include <AI\Behavior\RangedBehavior.h>
#include <Misc\RandomGenerator.h>
#include <Misc\ComboMachine.h>

using namespace Logic;

const int EnemyNecromancer::SPEED_AB1 = 125,
          EnemyNecromancer::SPEED_AB2 = 20,
          EnemyNecromancer::MAX_SPAWNED_MINIONS = 4,
          EnemyNecromancer::BASE_DAMAGE = 1,
          EnemyNecromancer::MAX_HP = 200,
          EnemyNecromancer::SCORE = 50;
const float EnemyNecromancer::BASE_SPEED = 7.5f;

EnemyNecromancer::EnemyNecromancer(btRigidBody* body, btVector3 halfExtent)
    : Enemy(Resources::Models::Necromancer, body, halfExtent, MAX_HP, BASE_DAMAGE,
        BASE_SPEED, EnemyType::NECROMANCER, 0, { 0.f, -1.9f, 0.f }) {
    setBehavior(RANGED);
    addCallback(ON_DEATH, [&](CallbackData data) -> void {
        ComboMachine::Get().kill(SCORE);
        RandomGenerator::singleton().getRandomInt(0, 1) ? 
            SpawnTrigger(2, getPositionBT() - btVector3(0.f, data.caller->getRigidBody()->getCollisionShape()->getLocalScaling().y() * 1.5f, 0.f), std::vector<int>{ StatusManager::AMMO_PICK_UP_PRIMARY }) : 
            SpawnTrigger(3, getPositionBT() - btVector3(0.f, data.caller->getRigidBody()->getCollisionShape()->getLocalScaling().y() * 1.5f, 0.f), std::vector<int>{ StatusManager::AMMO_PICK_UP_SECONDARY });
    });
    m_spawnedMinions = 0;

    createAbilities();

    getSoundSource()->autoPlaySFX(Sound::SFX::ENEMY_AMBIENT_2, 6500, 500, 1.f, 0.10f);
    light.color = DirectX::SimpleMath::Color(0.5f, 0.0f, 1.0f);
    light.intensity = 0.8f;
    light.range = 7.0f;
}

EnemyNecromancer::~EnemyNecromancer()
{
}

void EnemyNecromancer::createAbilities()
{
    // AB 1: Summon necromancer
    AbilityData data;
    data.cooldown = 1300.f;
    data.duration = 0.f;
    data.randomChanche = 10;

    static Graphics::ParticleEffect necroTrail = Graphics::FXSystem->getEffect("NecroProjTrail");
    ProjectileData pdata;
    pdata.effect = necroTrail;
    pdata.hasEffect = true;
    pdata.effectVelocity = false;
    pdata.effectActivated = true;

    auto onTick = [&](Player &player, Ability &ab) -> void {};

    auto onUse = [=](Player &player, Ability &ab) -> void {
        Projectile *pj = shoot(((player.getPositionBT() - getPositionBT()) + btVector3{ 0, 80, 0 }).normalize(), pdata, (float)SPEED_AB2, 2.5f, 0.6f);
		if (pj)
		{
			pj->addCallback(ON_DESTROY, [&](CallbackData &data) -> void {
				if (m_spawnedMinions < MAX_SPAWNED_MINIONS)
				{
                    data.caller->getSoundSource()->playSFX(Sound::SFX::NECROMANCER_SPAWN);
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

    ab1 = Ability(data, onTick, onUse);
    
    // AB 2: Ice Lance
    data.duration = 5000.f;
    data.cooldown = 6000.f;
    data.randomChanche = 5;

    ab2ProjData.meshID = Resources::Models::Ammocrystal;
    ab2ProjData.speed = SPEED_AB1;
    ab2ProjData.ttl = 25000.f;
    ab2ProjData.gravityModifier = 0.f;
    ab2ProjData.enemyBullet = true;
    ab2ProjData.damage = getBaseDamage();
    ab2ProjData.scale = 1.5f;

    auto onUse2 = [&](Player &player, Ability &ab) -> void {
        increaseCallbackEntities();
        ab2Projectile = SpawnProjectile(ab2ProjData, getPositionBT(), { 0.f, 0.f, 0.f }, *this);

        if (!ab2Projectile) ab.cancel();
        else
        {
            ab2Projectile->addCallback(ON_DESTROY, [&](CallbackData &data) -> void {
                ab.cancel();
                decreaseCallbackEntities();
            });
        }
    };
    auto onTick2 = [&](Player &player, Ability &ab) -> void {
        ab2Projectile->getRigidBody()->getWorldTransform().setOrigin(getPositionBT() +
            btVector3(2.5f, 4.f, 0.f) * btScalar((1.02f - (ab.getCurrentDuration() / ab.getData().duration))));

        if (ab.getCurrentDuration() <= 0.f)
        {
            ab2Projectile->start((player.getPositionBT() - ab2Projectile->getPositionBT()).normalized(), getStatusManager());
        }
    };

    ab2 = Ability(data, onTick2, onUse2);
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
    if (m_spawnedMinions < MAX_SPAWNED_MINIONS) ab1.useAbility(target);
    ab2.useAbility(target);
}
