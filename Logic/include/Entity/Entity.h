#ifndef ENTITY_H
#define ENTITY_H

#include <functional>
#include <unordered_map>

#include "PhysicsObject.h"
#include "StatusManager.h"
#include <AI\EnemyType.h>
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
    public:
        enum EntityEvent { ON_DEATH, ON_DAMAGE_TAKEN, ON_DAMAGE_GIVEN, ON_COLLISION };
        struct CallbackData {
            Entity *caller; // The entity that called the callback
            union
            {
                std::int64_t data64;
                std::int32_t data32;
                std::intptr_t dataPtr;
            };
        };
        typedef std::function<void(CallbackData)> callback;

		Entity(btRigidBody* body, btVector3 halfExtent, Graphics::ModelID modelID);
		Entity(const Entity& other) = delete;
		Entity* operator=(const Entity& other) = delete;

		virtual ~Entity();

        virtual void setSpawnFunctions(std::function<Projectile*(ProjectileData& pData,
            btVector3 position, btVector3 forward, Entity& shooter)> spawnProjectile,
            std::function<Enemy*(ENEMY_TYPE type, btVector3 &pos, std::vector<int> effects)> SpawnEnemy,
            std::function<Trigger*(int id, btVector3 const &pos, std::vector<int> &effects)> spawnTriggery);

		virtual void clear();
		virtual void update(float deltaTime);
		virtual void updateSound(float deltaTime);

		virtual void affect(int stacks, Effect const &effect, float deltaTime) = 0;
		virtual void upgrade(Upgrade const &upgrade);

        // Use this to add a callback, use lambda for ez code. Don't hardcode on collision stuff,
        // use this or buffs
        void addCallback(EntityEvent entityEvent, callback callback);
        bool hasCallback(EntityEvent entityEvent) const;

		StatusManager& getStatusManager();
		void setStatusManager(StatusManager& statusManager);

		Sound::SoundSource* getSoundSource();
        std::unordered_map<EntityEvent, std::function<void (CallbackData&)>>& getCallbacks();
	protected:
        // Functions to spawn other things
        std::function<Projectile*(ProjectileData& pData, btVector3 position,
            btVector3 forward, Entity& shooter)>               SpawnProjectile;
        std::function<Enemy*(ENEMY_TYPE type, btVector3 &pos,
            std::vector<int> effects)>                         SpawnEnemy;
        std::function<Trigger*(int id, btVector3 const &pos,
            std::vector<int> &effects)>                        SpawnTrigger;
	private:
        Sound::SoundSource* m_soundSource;
		StatusManager m_statusManager;
        // change functions to linked list if many callbacks is wanted, but i don't see it being necessary
        std::unordered_map<EntityEvent, std::function<void (CallbackData&)>> m_callbacks;
	};

	/* Temp class cuz the fucking entity is abstract */
	class Speaker : public Entity
	{
	public :
		Speaker(btRigidBody* body, btVector3 halfExtent, Graphics::ModelID modelID)
			: Entity(body, halfExtent, modelID) { }
		void affect(int stacks, Effect const &effect, float deltaTime) { }
		void onCollision(PhysicsObject & other, btVector3 contactPoint, float multiplier) { }
	};
}

#endif // !ENTITY_H
