#ifndef TRIGGER_H
#define TRIGGER_H

#include <Graphics\include\RenderQueue.h>

#include <Player\Player.h>
#include <Entity\Entity.h>

#pragma region ClassDesc
	/*
		CLASS: Trigger (Bad name TODO: change)
		AUTHOR: Lukas Westling

		This class is a Trigger, with its attributes.
		(Not activated flag, that is in the TriggerManager
		due to cachelines)
	*/
#pragma endregion

#define TRIGGER_MASS		0.f		// Should always be 0.f, unless debugging
#define TRIGGER_IS_SENSOR	true	// Should always be false, unless debugging

namespace Logic
{
	class Trigger : public Entity
	{
		public:
			Trigger(Resources::Models::Files modelID, btRigidBody* body, btVector3 halfExtent, float cooldown, bool reusable);
			virtual ~Trigger();

			void addUpgrades(const std::vector<StatusManager::UPGRADE_ID>& upgrades);
			void addEffects(const std::vector<StatusManager::EFFECT_ID>& effects);
			void affect(int stacks, Effect const & effect, float deltaTime);

			void updateSpecific(float deltaTime);
			void onCollision(PhysicsObject& other, btVector3 contactPoint, float dmgMultiplier);

			bool getShouldRemove() const;
			bool getIsActive() const;
			bool getIsReusable() const;
			float getCooldown() const;
			void setShouldRemove(bool remove);
			void setIsActive(bool active);
			void setIsReusable(bool	reusable);
			void setCooldown(float cooldown);

            void render() const;

		private:
			bool m_remove;
			bool m_active;
			bool m_reusable;
			float m_cooldown;
			float m_maxCooldown;

            LightRenderInfo light;
            StaticRenderInfo animatedRenderInfo;
	};
}

#endif