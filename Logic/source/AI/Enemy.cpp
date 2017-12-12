#include <AI\Enemy.h>
#include <AI\Behavior\TestBehavior.h>
#include <AI\Behavior\RangedBehavior.h>
#include <AI\Behavior\MeleeBehavior.h>
#include <AI\Behavior\BigBadBehavior.h>
#include <AI\Behavior\StayBehavior.h>

#include <Player\Player.h>
#include <Projectile\ProjectileStruct.h>
#include <Projectile\Projectile.h>

using namespace Logic;
const int Enemy::MIN_Y = -80.f;
const float Enemy::MAX_TARGET_Y = 4.4;

Enemy::Enemy(Resources::Models::Files modelID, btRigidBody* body, btVector3 halfExtent, int health, int baseDamage, float moveSpeed, EnemyType enemyType, int animationId, btVector3 modelOffset)
: Entity(body, halfExtent, modelOffset), animatedModel(modelID, "")
{
	m_behavior = nullptr;

	m_health = health;
	m_maxHealth = health;
	m_baseDamage = baseDamage;
	m_moveSpeed = moveSpeed;
	m_enemyType = enemyType;
	m_bulletTimeMod = 1.f;
    m_moveSpeedMod = 1.f;

    m_nrOfCallbacksEntities = 0;

    m_stunned = false;
    m_fireTimer = 0.f;
    m_blinkTimer = -1.0f;

    maxAnimationTime = 0.f;

    addCallback(ON_DAMAGE_TAKEN, [&](CallbackData &data) -> void {
        m_blinkTimer = 100.0f;
        getSoundSource()->playSFX(Sound::SFX::ENEMY_HIT, 1.f, 0.2f);
    });

    addCallback(ON_DEATH, [&](CallbackData &data)
    {
        Graphics::FXSystem->addEffect("DeathEffect", data.caller->getPosition());
        getSoundSource()->playSFX(Sound::SFX::ENEMY_DEATH, 1.f, 0.25f);
    });
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
        case BOSS_BADDIE:
                m_behavior = newd BigBadBehavior();
            break;
        case STAY:
            m_behavior = newd StayBehavior();
            break;
		default:
				m_behavior = newd TestBehavior();
			break;
	}
}

void Enemy::setEnemyType(EnemyType id)
{
	m_enemyType = id;
}

Enemy::~Enemy() {
	if (m_behavior)
		delete m_behavior;
}

void Enemy::update(Player &player, float deltaTime, std::vector<Enemy*> const &closeEnemies) {
	Entity::update(deltaTime);

    // update behavior
    if (!m_stunned || getEnemyType() == EnemyType::BOSS_1) // quick fix
    {
        m_behavior->update(*this, closeEnemies, player, deltaTime);
    }

    // helps them come up to the player
    if (player.getPositionBT().y() < MAX_TARGET_Y) {
        getRigidBody()->setGravity({ 0.f, -9.82f * 25.f, 0.f });
    }
    else
    {
        getRigidBody()->setGravity({ 0.f, -9.82f * 3.f, 0.f });
    }

    // update specifc
	updateSpecific(player, deltaTime);

    // Rotation toward their moving direction
    btVector3 dir = getRigidBody()->getLinearVelocity();
    float yaw = atan2(dir.getX(), dir.getZ());
    m_transform->setRotation(btQuaternion(yaw, 0.f, 0));

    // Update animation and position
    updateAnimation(deltaTime);

    m_moveSpeedMod = 1.f;
	m_bulletTimeMod = 1.f; // Reset effect variables, should be in function if more variables are added.
    light.position = getPosition();

    // out of bounds insta kill
    if (getPositionBT().y() < MIN_Y || getPositionBT().length2() > 62500.f)
        damage(m_health);

    // blinking when damaged
    if (m_blinkTimer > 0)
    {
        animatedModel.set_color(DirectX::SimpleMath::Vector3(10.0f, 0.0f, 0.0f));
        m_blinkTimer -= deltaTime;
    }
    else
    {
        animatedModel.set_color(DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f));
    }
}

void Enemy::updateAnimation(float deltaTime)
{
    animatedModel.set_transform(getModelTransformMatrix());
    animatedModel.update(deltaTime);
}

void Enemy::debugRendering()
{
	m_behavior->getPath().renderDebugging(getPosition());
}

void Enemy::increaseCallbackEntities()
{
    m_nrOfCallbacksEntities++;
}

void Enemy::decreaseCallbackEntities()
{
    m_nrOfCallbacksEntities--;
}

bool Enemy::hasCallbackEntities()
{
    return m_nrOfCallbacksEntities > 0;
}

void Enemy::damage(int damage)
{
    if (damage <= 0) return;

	m_health -= damage;
    m_blinkTimer = 100.0f;

    callback(ON_DAMAGE_TAKEN, CallbackData { this, static_cast<int32_t> (damage) });
    if (m_health <= 0 && m_health + damage > 0)
    {
        callback(ON_DEATH, CallbackData{ this, static_cast<int32_t> (damage) });
    }
}

void Enemy::affect(int stacks, Effect const &effect, float dt) 
{
	auto flags = effect.getStandards()->flags;
    m_moveSpeedMod = 1.0f;

    if (flags & Effect::EFFECT_MODIFY_HP)
        m_health += static_cast<int> (effect.getModifiers()->modifyHP);
    if (flags & Effect::EFFECT_ON_FIRE)
    {
        m_fireTimer += dt;

        if (m_fireTimer >= 1000.0f)
        {
            damage(static_cast<int> (effect.getModifiers()->modifyDmgTaken) * stacks);
            m_fireTimer -= 1000.0f;
        }
    }
    if (flags & Effect::EFFECT_FREEZE_DMG)
    {
        m_fireTimer += dt;

        if (m_fireTimer >= 1000.0f)
        {
            damage(static_cast<int> (effect.getModifiers()->modifyDmgTaken));

            m_fireTimer -= 1000.0f;
        }
    }
    if (flags & Effect::EFFECT_KILL)
        damage(m_health);
	if (flags & Effect::EFFECT_BULLET_TIME)
		m_bulletTimeMod = std::pow(effect.getSpecifics()->isBulletTime, stacks);
    if (flags & Effect::EFFECT_IS_FROZEN)
        m_moveSpeedMod *= std::pow(effect.getSpecifics()->isFreezing, stacks);
    if (flags & Effect::EFFECT_IS_STUNNED)
        m_stunned = true;
    if (flags & Effect::EFFECT_MOVE_FASTER)
       m_moveSpeedMod *= std::pow(effect.getModifiers()->modifyMovementSpeed, stacks);
    if (flags & Effect::EFFECT_MOVE_SLOWER)
       m_moveSpeedMod *= std::pow(effect.getModifiers()->modifyMovementSpeed, stacks);
}

void Logic::Enemy::onEffectAdd(int stacks, Effect const & effect)
{
    long long flags = effect.getStandards()->flags;

    if (flags & Effect::EFFECT_DAMAGE_ONCE)
    {
        damage(effect.getModifiers()->modifyDmgTaken);
    }
}

void Enemy::onEffectEnd(int stacks, Effect const & effect)
{
    long long flags = effect.getStandards()->flags;

    if (flags & Effect::EFFECT_ON_FIRE)
    {
        m_fireTimer = 0;
    }
    if (flags & Effect::EFFECT_BULLET_TIME)
    {
        //m_bulletTimeMod = 1.f;
    }
    if (flags & Effect::EFFECT_IS_FROZEN)
    {
        m_moveSpeedMod = 1.f;
    }
    if (flags & Effect::EFFECT_IS_STUNNED)
    {
        m_stunned = false;
    }
    if (flags & Effect::EFFECT_MOVE_FASTER)
    {
        m_moveSpeedMod = 1.f;
    }
    if (flags & Effect::EFFECT_MOVE_SLOWER)
    {
        m_moveSpeedMod = 1.f;
    }
}

int Enemy::getHealth() const
{
	return m_health;
}

int Enemy::getMaxHealth() const
{
	return m_maxHealth;
}

int Enemy::getBaseDamage() const
{
	return m_baseDamage;
}

float Enemy::getMoveSpeed() const
{
	return m_moveSpeed * getSpeedMod();
}

float Enemy::getSpeedMod() const
{
    return m_bulletTimeMod * m_moveSpeedMod;
}

EnemyType Enemy::getEnemyType() const
{
	return m_enemyType;
}

Projectile* Enemy::shoot(btVector3 dir, Resources::Models::Files id, float speed, float gravity, btVector3 scale, bool sensor)
{
	ProjectileData data;

	data.damage = getBaseDamage();
	data.meshID = id;
    data.shouldRender = true;
	data.speed = speed;
    data.ttl = 10000;
    data.gravityModifier = gravity;
	data.hitboxScale = scale;
    data.modelScale = scale;
    data.enemyBullet = true;
    data.isSensor = sensor;

    Projectile* pj = SpawnProjectile(data, getPositionBT(), dir, *this);
    
    if (pj)
    {
        increaseCallbackEntities();
        pj->addCallback(ON_DESTROY, [&](CallbackData &data) -> void {
            decreaseCallbackEntities();
        });
        if (hasCallback(ON_DAMAGE_GIVEN))
        {
            pj->addCallback(ON_DAMAGE_GIVEN, [&](CallbackData &data) -> void {
                callback(ON_DAMAGE_GIVEN, data);
            });
        }
    }
	
    return pj;
}

Projectile * Logic::Enemy::shoot(btVector3 dir, ProjectileData data, float speed, float gravity, btVector3 scale, bool sensor)
{
    data.damage = getBaseDamage();
    data.shouldRender = false;
    data.speed = speed;
    data.ttl = 10000;
    data.gravityModifier = gravity;
    data.hitboxScale = scale;
    data.modelScale = scale;
    data.enemyBullet = true;
    data.isSensor = sensor;

    Projectile* pj = SpawnProjectile(data, getPositionBT(), dir, *this);

    if (pj)
    {
        increaseCallbackEntities();
        pj->addCallback(ON_DESTROY, [&](CallbackData &data) -> void {
            decreaseCallbackEntities();
        });
        if (hasCallback(ON_DAMAGE_GIVEN))
        {
            pj->addCallback(ON_DAMAGE_GIVEN, [&](CallbackData &data) -> void {
                callback(ON_DAMAGE_GIVEN, data);
            });
        }
    }

    return pj;
}

Behavior* Enemy::getBehavior() const
{
	return this->m_behavior;
}

AnimatedModel &Logic::Enemy::getAnimatedModel()
{
    return animatedModel;
}

void Enemy::render() const
{
    renderSpecific();
    animatedModel.render();
    QueueRender(light);
}
