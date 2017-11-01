#include <AI\Enemy.h>
#include <AI\Behavior\TestBehavior.h>
#include <AI\Behavior\RangedBehavior.h>
#include <AI\Behavior\MeleeBehavior.h>

#include <Player\Player.h>
#include <Projectile\ProjectileStruct.h>
#include <Projectile\Projectile.h>

using namespace Logic;

Enemy::Enemy(Graphics::ModelID modelID, btRigidBody* body, btVector3 halfExtent, float health, float baseDamage, float moveSpeed, ENEMY_TYPE enemyType, int animationId)
: Entity(body, halfExtent, modelID)
{
	m_behavior = nullptr;

	m_health = health;
	m_maxHealth = health;
	m_baseDamage = baseDamage;
	m_moveSpeed = moveSpeed;
	m_enemyType = enemyType;
	m_bulletTimeMod = 1.f;
    m_moveSpeedMod = 1.f;
}

void Enemy::setBehavior(BEHAVIOR_ID id)
{
	if (m_behavior)
		delete m_behavior;
	m_behavior = nullptr;

	switch (id) 
	{
		case TEST:
				m_behavior = newd TestBehavior();
			break;
		case RANGED:
				m_behavior = newd RangedBehavior();
			break;
        case MELEE:
                m_behavior = newd MeleeBehavior();
            break;
		default:
				m_behavior = newd TestBehavior();
			break;
	}
}

void Enemy::setEnemyType(ENEMY_TYPE id)
{
	m_enemyType = id;
}

Enemy::~Enemy() {
	if (m_behavior)
		delete m_behavior;
}

void Enemy::update(Player const &player, float deltaTime, std::vector<Enemy*> const &closeEnemies) {
	Entity::update(deltaTime);
	updateSpecific(player, deltaTime);

	m_behavior->update(*this, closeEnemies, player, deltaTime); // BEHAVIOR IS NOT DONE, FIX LATER K

    m_moveSpeedMod = 1.f;
	m_bulletTimeMod = 1.f; // Reset effect variables, should be in function if more variables are added.
}

void Enemy::debugRendering(Graphics::Renderer & renderer)
{
	m_behavior->getPath().renderDebugging(renderer, getPosition());
}

void Enemy::damage(float damage)
{
	m_health -= damage;

    if (hasCallback(ON_DAMAGE_TAKEN))
        getCallbacks()[ON_DAMAGE_TAKEN](CallbackData { this, static_cast<int32_t> (damage) });

    if (m_health <= 0 && m_health + damage > 0)
        if (hasCallback(ON_DEATH))
            getCallbacks()[ON_DEATH](CallbackData {this, static_cast<int32_t> (damage)});
}

void Enemy::affect(int stacks, Effect const &effect, float dt) 
{
	auto flags = effect.getStandards()->flags;

	if (flags & Effect::EFFECT_KILL)
		damage(m_health);
	if (flags & Effect::EFFECT_ON_FIRE)
		damage(effect.getModifiers()->modifyDmgTaken * dt);
	if (flags & Effect::EFFECT_BULLET_TIME)
		m_bulletTimeMod *= std::pow(effect.getSpecifics()->isBulletTime, stacks);
    if (flags & Effect::EFFECT_IS_FROZEN)
        m_moveSpeedMod *= std::pow(effect.getSpecifics()->isFreezing, stacks);
}

float Enemy::getHealth() const
{
	return m_health;
}

float Enemy::getMaxHealth() const
{
	return m_maxHealth;
}

float Enemy::getBaseDamage() const
{
	return m_baseDamage;
}

float Enemy::getMoveSpeed() const
{
	return m_moveSpeed * m_bulletTimeMod * m_moveSpeedMod;
}

ENEMY_TYPE Enemy::getEnemyType() const
{
	return m_enemyType;
}

Projectile* Enemy::shoot(btVector3 dir, Graphics::ModelID id, float speed, float gravity, float scale)
{
	ProjectileData data;

	data.damage = getBaseDamage();
	data.meshID = id;
	data.speed = speed;
    data.ttl = 20000;
    data.gravityModifier = 2.5;
	data.scale = scale;
    data.enemyBullet = true;
    data.isSensor = true;

    Projectile* pj = SpawnProjectile(data, getPositionBT(), dir, *this);
    
    if (hasCallback(ON_DAMAGE_GIVEN))
        pj->addCallback(ON_DAMAGE_GIVEN, getCallbacks()[ON_DAMAGE_GIVEN]);
	
    return pj;
}

Behavior* Enemy::getBehavior() const
{
	return m_behavior;
}
