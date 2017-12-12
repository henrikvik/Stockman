#ifndef ENTITY_H
#define ENTITY_H

#include <functional>
#include <unordered_map>

#include "PhysicsObject.h"
#include "StatusManager.h"
#include <AI\EnemyType.h>
#include <Graphics\include\RenderQueue.h>
#include <btBulletCollisionCommon.h>
#include <Misc/Sound/NoiseMachine.h>
#include <Misc/Sound/SoundSource.h>

namespace Sound
{
    class SoundSource;
}

namespace Logic
{
    class Projectile;
    class Trigger;
    class Enemy;
    struct ProjectileData;

	class Entity : public PhysicsObject
	{
    typedef std::function<Projectile*(ProjectileData& pData, btVector3 position, btVector3 forward, Entity& shooter)> ProjectileFunc;
    typedef std::function<Enemy*(EnemyType type, btVector3 &pos, std::vector<int> effects)> EnemyFunc;
    typedef std::function<Trigger*(int id, btVector3 const &pos, std::vector<int> &effects)> TriggerFunc;

    public:
        // Use ON_DESTROY to prevent crashes
        enum EntityEvent { ON_DEATH, ON_DAMAGE_TAKEN, ON_DAMAGE_GIVEN, ON_COLLISION, ON_DESTROY };
        struct CallbackData {
            Entity *caller; // The entity that called the callback
            union
            {
                std::int32_t data32;
                std::intptr_t dataPtr;
            };
        };
        // An unordered map with a list of callbacks
        typedef std::function<void(CallbackData&)> Callback;

        Entity(btRigidBody* body, btVector3 halfExtent, btVector3 modelOffset = { 0.f, 0.f, 0.f });
		Entity(const Entity& other) = delete;
		Entity* operator=(const Entity& other) = delete;

		virtual ~Entity();

        virtual void setSpawnFunctions(ProjectileFunc spawnProjectile, EnemyFunc SpawnEnemy, TriggerFunc spawnTrigger);

		virtual void clear();
		virtual void update(float deltaTime);
		virtual void updateSound(float deltaTime);

		virtual void affect(int stacks, Effect const &effect, float deltaTime) = 0;
        virtual void onEffectAdd(int stacks, Effect const &effect) {};
        virtual void onEffectEnd(int stacks, Effect const &effect) {};
        virtual void onUpgradeAdd(int stacks, Upgrade const &upgrade) {};
		
        virtual void upgrade(StatusManager::UPGRADE_ID id, int stacks = 1);

        virtual void render() const = 0;

        void callback(EntityEvent entityEvent, CallbackData &data);

        // Use this to add a callback, use lambda for ez code. Don't hardcode on collision stuff,
        // use this or buffs
        void addCallback(EntityEvent entityEvent, Callback callback);
        bool hasCallback(EntityEvent entityEvent) const;
        void clearCallbacks(bool callOnDestroy = false);

		StatusManager& getStatusManager();
		void setStatusManager(StatusManager& statusManager);

		Sound::SoundSource* getSoundSource();
	protected:
        // Functions to spawn other things
        std::function<Projectile*(ProjectileData& pData, btVector3 position,
            btVector3 forward, Entity& shooter)>               SpawnProjectile;
        std::function<Enemy*(EnemyType type, btVector3 &pos,
            std::vector<int> effects)>                         SpawnEnemy;
        std::function<Trigger*(int id, btVector3 const &pos,
            std::vector<int> &effects)>                        SpawnTrigger;
	private:
        Sound::SoundSource* m_soundSource;
		StatusManager m_statusManager;
        // change functions to linked list if many callbacks is wanted, but i don't see it being necessary
        std::unordered_map<EntityEvent, std::vector<Callback>> m_callbacks;
	};
}

#endif // !ENTITY_H
