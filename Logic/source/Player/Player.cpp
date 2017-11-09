#include "Player/Player.h"
#include <DebugDefines.h>

#include <Misc\ComboMachine.h>

#include <AI\EnemyTest.h>
#include <AI\Trigger.h>

#include <Projectile\ProjectileManager.h>

#include <Player\Weapon\WeaponManager.h>
#include <Player\Weapon\Weapon.h>

#include <Player\Skill\SkillManager.h>
#include <Player\Skill\Skill.h>

#include <Misc\Sound\NoiseStructs.h>

#include <Engine\Profiler.h>
#include <Engine\DebugWindow.h>
#include <Graphics\include\MainCamera.h>
#include <Graphics\include\Device.h>

using namespace Logic;

Player::Player(Resources::Models::Files modelID, btRigidBody* body, btVector3 halfExtent)
: Entity(body, halfExtent)
{
    m_weaponManager = newd WeaponManager();
    m_skillManager = newd SkillManager();
    m_listenerData = newd Sound::ListenerData();
    m_pMovement = newd PlayerMovement();
}

Player::~Player()
{
	clear();
}

void Player::init(Physics* physics, ProjectileManager* projectileManager)
{
	m_weaponManager->init(projectileManager);
	m_skillManager->init(physics, projectileManager);
	m_physPtr = physics;

	// Stats
	m_hp = PLAYER_STARTING_HP;
    currentWeapon = 0;

	// Default mouse sensetivity, lookAt
	m_camYaw = 90;
	m_camPitch = 5;

	// Default controlls
	m_moveLeft = DirectX::Keyboard::Keys::A;
	m_moveRight = DirectX::Keyboard::Keys::D;
	m_moveForward = DirectX::Keyboard::Keys::W;
	m_moveBack = DirectX::Keyboard::Keys::S;
	m_jump = DirectX::Keyboard::Keys::Space;
	m_switchWeaponOne = DirectX::Keyboard::Keys::D1;
	m_switchWeaponTwo = DirectX::Keyboard::Keys::D2;
	m_switchWeaponThree = DirectX::Keyboard::Keys::D3;
	m_reloadWeapon = DirectX::Keyboard::Keys::R;
    m_useSkillPrimary = DirectX::Keyboard::Keys::F;
    m_useSkillSecondary = DirectX::Keyboard::Keys::E;
    m_useSkillTertiary = DirectX::Keyboard::Keys::G;
	m_listenerData->update({ 0, 0, 0 }, { 0, 1, 0 }, m_pMovement->getForwardBT(), m_pMovement->getCharController()->getGhostObject()->getWorldTransform().getOrigin());
}

void Player::clear()
{
	m_weaponManager->clear();
    delete m_weaponManager;
    delete m_skillManager;
    delete m_listenerData;
    delete m_pMovement;
}

void Player::reset()
{
    getTransform().setOrigin({0, 6, 0});
	m_weaponManager->reset();
	m_hp = 3;

    //temp? probably
    Global::mainCamera->update(getPosition(), DirectX::SimpleMath::Vector3(m_pMovement->getForwardBT()), Global::context);
    static SpecialEffectRenderInfo info;
    info.type = info.Snow;
    info.restart = true;

    RenderQueue::get().queue(&info);
}

void Player::onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier)
{
    if (!m_pMovement->getGodMode())
    {
        if (Projectile* p = dynamic_cast<Projectile*>(&other))	onCollision(*p);										// collision with projectile
        else if (Trigger* t = dynamic_cast<Trigger*>(&other)) {}														// collision with trigger
        else if (Enemy *e = dynamic_cast<Enemy*> (&other))
        {
            int stacks = getStatusManager().getStacksOfEffectFlag(Effect::EFFECT_FLAG::EFFECT_CONSTANT_PUSH_BACK);
            e->getRigidBody()->applyCentralForce((getPositionBT() - e->getPositionBT()).normalize() * static_cast<btScalar> (stacks));
            stacks = getStatusManager().getStacksOfEffectFlag(Effect::EFFECT_FLAG::EFFECT_CONSTANT_DAMAGE_ON_CONTACT);
            e->damage(2.f * stacks); // replace 1 with the player damage when it is better
        }
    }
}

void Player::onCollision(Projectile& other)
{
	if (other.getProjectileData().enemyBullet)
		takeDamage((int)other.getProjectileData().damage);
}

void Player::affect(int stacks, Effect const &effect, float deltaTime)
{
	long long flags = effect.getStandards()->flags;

	if (flags & Effect::EFFECT_MODIFY_MOVEMENTSPEED)
	{
        m_pMovement->forceJump();
	}

    if (flags & Effect::EFFECT_MODIFY_HP)
    {
        m_hp += static_cast<int> (effect.getModifiers()->modifyHP);
    }

	if (flags & Effect::EFFECT_MODIFY_AMMO)
	{
        Weapon* wp = nullptr;
        if(effect.getSpecifics()->ammoType == 0)
		    wp = m_weaponManager->getFirstWeapon().first;
        else if(effect.getSpecifics()->ammoType == 1)
            wp = m_weaponManager->getSecondWeapon().first;

        if (wp)
        {
            int magSize = wp->getMagSize();
            int currentAmmo = wp->getAmmo();
            if (currentAmmo + magSize > wp->getAmmoCap())
                wp->setAmmo(wp->getAmmoCap());
            else
                wp->setAmmo(currentAmmo + magSize);
        }
	}
}

void Player::upgrade(Upgrade const & upgrade)
{
	long long flags = upgrade.getTranferEffects();

	if (flags & Upgrade::UPGRADE_INCREASE_MAGSIZE)
	{

	}
}

void Player::updateSound(float deltaTime)
{
	// Update sound position
	btVector3 pos = getPositionBT();
	btVector3 vel = m_pMovement->getCharController()->getLinearVelocity();
	getSoundSource()->pos = { pos.x(), pos.y(), pos.z() };
    getSoundSource()->vel = { vel.x(), vel.y(), vel.z() };
    getSoundSource()->update(deltaTime);
}

void Player::saveToFile()
{

}

void Player::readFromFile()
{

}

void Player::takeDamage(int damage, bool damageThroughProtection)
{
    if (!m_pMovement->getGodMode())
    {
        if (damageThroughProtection ||
            getStatusManager().getStacksOfEffectFlag(Effect::EFFECT_FLAG::EFFECT_CONSTANT_INVINC) == 0)
        {
            getSoundSource()->playSFX(Sound::SFX::NECROMANCER_DEATH);
            m_hp -= damage;
        }
    }
}

int Player::getHP() const
{
	return m_hp;
}

void Player::updateSpecific(float deltaTime)
{
	Player::update(deltaTime);

	// Updates listener info for sounds
	btVector3 up		= { 0, 1, 0 };
	btVector3 forward	= m_pMovement->getForwardBT();
	btVector3 right		= up.cross(forward);
	btVector3 actualUp	= right.cross(forward);
	m_listenerData->update({ 0, 0, 0 }, actualUp.normalize(), m_pMovement->getForwardBT(), m_pMovement->getCharController()->getGhostObject()->getWorldTransform().getOrigin());

	// Get Mouse and Keyboard states for this frame
	DirectX::Keyboard::State ks = DirectX::Keyboard::Get().GetState();
	DirectX::Mouse::State ms = DirectX::Mouse::Get().GetState();

	// Temp for testing
	if (ks.IsKeyDown(DirectX::Keyboard::B))
	{
        m_pMovement->warpToOrigin();
	}

	// TEMP FREE MOVE
    if (ks.IsKeyDown(DirectX::Keyboard::N) && !m_pMovement->getNoClip())
	{
        m_pMovement->activeNoClip();
	}
	else if (ks.IsKeyDown(DirectX::Keyboard::M) && m_pMovement->getNoClip())
	{
        m_pMovement->deactivateNoClip();
	}

	// Movement
    int x = DirectX::Mouse::Get().GetState().x;
    int y = DirectX::Mouse::Get().GetState().y;

    if (ms.positionMode == DirectX::Mouse::MODE_RELATIVE)
        m_pMovement->moveMouse(x, y);

    if (ks.IsKeyDown(m_jump))
        m_pMovement->wantToJump();
    else if (ks.IsKeyUp(m_jump))
        m_pMovement->doesNotWantToJump();

    // Get movement input
    int directionFlag = 0;
    if (ks.IsKeyDown(m_moveLeft))       directionFlag |= DIRECTION_FLAG::DIR_LEFT;
    if (ks.IsKeyDown(m_moveRight))      directionFlag |= DIRECTION_FLAG::DIR_RIGHT;
    if (ks.IsKeyDown(m_moveForward))    directionFlag |= DIRECTION_FLAG::DIR_FORWARD;
    if (ks.IsKeyDown(m_moveBack))       directionFlag |= DIRECTION_FLAG::DIR_BACKWARD;
    m_pMovement->moveDirection(directionFlag);

    m_pMovement->update(deltaTime);

	// Weapon swap
    if (ks.IsKeyDown(m_switchWeaponOne))
    {
        getSoundSource()->playSFX(Sound::SFX::SWOOSH);
        m_weaponManager->switchWeapon(0);
        currentWeapon = 0;
    }
		
    if (ks.IsKeyDown(m_switchWeaponTwo))
    {
        getSoundSource()->playSFX(Sound::SFX::SWOOSH);
        m_weaponManager->switchWeapon(1);
        currentWeapon = 1;
    }
		
    if (ks.IsKeyDown(m_switchWeaponThree))
    {
        getSoundSource()->playSFX(Sound::SFX::SWOOSH);
        m_weaponManager->switchWeapon(2);
        currentWeapon = 2;
    }
		
	// Skills
    PROFILE_BEGIN("SkillManager");
    forward = m_pMovement->getForwardBT();
    if (ks.IsKeyDown(m_useSkillPrimary))
        m_skillManager->use(SkillManager::ID::PRIMARY, forward, *this);
    if (ks.IsKeyUp(m_useSkillPrimary))
        m_skillManager->release(SkillManager::ID::PRIMARY);
    if (ks.IsKeyDown(m_useSkillSecondary))
        m_skillManager->use(SkillManager::ID::SECONDARY, forward, *this);
    if (ks.IsKeyUp(m_useSkillSecondary))
        m_skillManager->release(SkillManager::ID::SECONDARY);
    if (ks.IsKeyDown(m_useSkillTertiary))
        m_skillManager->use(SkillManager::ID::TERTIARY, forward, *this);
    if (ks.IsKeyUp(m_useSkillTertiary))
        m_skillManager->release(SkillManager::ID::TERTIARY);
    PROFILE_END();

	// Check if reloading
	if (!m_weaponManager->isReloading())
	{
		// Primary and secondary attack
		if (!m_weaponManager->isAttacking() && ms.positionMode == DirectX::Mouse::MODE_RELATIVE) //do i need to exclude more from relative mode?
		{
			btVector3 pos = getPositionBT() + m_pMovement->getForwardBT();
            if ((ms.leftButton))
            {
                getSoundSource()->playSFX(Sound::SFX::WEAPON_CUTLERY_PRIMARY, 1.f, 0.15f);
                m_weaponManager->usePrimary(pos, m_camYaw, m_camPitch, *this);
            }
            else if (ms.rightButton)
            {
                getSoundSource()->playSFX(Sound::SFX::WEAPON_CUTLERY_SECONDARY, 1.f, 0.05f);
                m_weaponManager->useSecondary(pos, m_camYaw, m_camPitch, *this);
            }
		}

		// Reload
		if (ks.IsKeyDown(m_reloadWeapon))
			m_weaponManager->reloadWeapon();
	}

    // Update weapon and skills
    m_weaponManager->update(deltaTime);
    m_skillManager->update(deltaTime);

   /* if (m_godMode)
    {*/
        static bool isNum = false;
        static bool wasNum = false;
        wasNum = isNum;
        isNum = ks.NumPad6;

        if (isNum && !wasNum)
            m_hp--;
    /*}*/

    Global::mainCamera->update(getPosition(), DirectX::SimpleMath::Vector3(m_pMovement->getForwardBT()), Global::context);
}

DirectX::SimpleMath::Matrix Player::getTransformMatrix() const
{
	// Making memory for a matrix
	float* m = newd float[4 * 16];

	// Getting this entity's matrix
	m_pMovement->getCharController()->getGhostObject()->getWorldTransform().getOpenGLMatrix((btScalar*)(m));

	// Translating to DirectX Math and assigning the variables
	DirectX::SimpleMath::Matrix transformMatrix(m);

	//Find the scaling matrix
	auto scale = DirectX::SimpleMath::Matrix::CreateScale(PLAYER_SIZE_RADIUS * 2, PLAYER_SIZE_HEIGHT * 2, PLAYER_SIZE_RADIUS * 2);

	// Deleting the old created variables from memory
	delete m;

	return scale * transformMatrix;
}

void Player::render() const
{
	// Drawing the actual player model (can be deleted later, cuz we don't need it, unless we expand to multiplayer)
//	Object::render(renderer);

	// Setting position of updated weapon and skill models
	m_weaponManager->setWeaponModel(getTransformMatrix(), DirectX::SimpleMath::Vector3(m_pMovement->getForwardBT()));
	//	m_skillManager->setWeaponModel(getTransformMatrix(), m_forward);

	// Drawing the weapon model
	m_weaponManager->render();
	m_skillManager->render();
}

Sound::ListenerData& Player::getListenerData()
{
	return *m_listenerData;
}

SkillManager* Player::getSkillManager()
{
    return m_skillManager;
}

PlayerMovement * Logic::Player::getPlayerMovement()
{
    return m_pMovement;
}

const Weapon* Player::getMainHand() const
{
    return m_weaponManager->getActiveWeapon();
}

const Weapon* Player::getOffHand() const
{
    return m_weaponManager->getInactiveWeapon();
}

const Skill* Player::getSkill(int id) const
{
    return m_skillManager->getSkill(id);
}

bool Player::isUsingMeleeWeapon() const
{
    return m_weaponManager->getActiveWeapon()->getAmmoConsumption() == 0;
}

int Player::getCurrentWeapon() const
{
    return currentWeapon;
}

void Player::setCurrentSkills(int first, int second)
{
    m_skillManager->switchToSkill({ SkillManager::SKILL(second), SkillManager::SKILL(first) });
    currentSkills[0] = first;
    currentSkills[1] = second;
}

int Player::getCurrentSkill1() const
{
    return currentSkills[1];
}

int Player::getCurrentSkill0() const
{
    return currentSkills[0];
}
