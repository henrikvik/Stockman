#ifndef ENTITY_H
#define ENTITY_H

#include <functional>
#include <unordered_map>

#include "PhysicsObject.h"
#include "StatusManager.h"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <Misc/Sound/NoiseMachine.h>
#include <Misc/Sound/SoundSource.h>

namespace Logic
{
	class Entity : public PhysicsObject
	{
    public:
        enum EntityEvent { ON_DEATH, ON_DAMAGE_TAKEN, ON_DAMAGE_GIVEN, ON_COLLISION };
        union CallbackData
        {
            std::int64_t data64;
            std::int32_t data32;
            std::intptr_t dataPtr;
        };
        typedef void(*callback)(CallbackData&);

		Entity(btRigidBody* body, btVector3 halfExtent, Graphics::ModelID modelID = Graphics::ModelID::CUBE);
		Entity(const Entity& other) = delete;
		Entity* operator=(const Entity& other) = delete;
		virtual ~Entity();

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

		SoundSource* getSoundSource();
        std::unordered_map<EntityEvent, std::function<void (CallbackData&)>>& getCallbacks();
	protected:
		SoundSource m_soundSource; // they'll never catch me
	private:
		StatusManager m_statusManager;
        // change functions to linked list if many callbacks is wanted, but i don't see it being necessary
        std::unordered_map<EntityEvent, std::function<void (CallbackData&)>> m_callbacks;
	};

	/* Temp class cuz the fucking entity is abstract */
	class Speaker : public Entity
	{
	public :
		Speaker(btRigidBody* body, btVector3 halfExtent, Graphics::ModelID modelID = Graphics::ModelID::CUBE)
			: Entity(body, halfExtent, modelID) { }
		void affect(int stacks, Effect const &effect, float deltaTime) { }
		void onCollision(PhysicsObject & other, btVector3 contactPoint, float multiplier) { }
	};
}

#endif // !ENTITY_H
