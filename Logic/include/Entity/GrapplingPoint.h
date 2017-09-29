#ifndef GRAPPLINGPOINT_H
#define GRAPPLINGPOINT_H

#include <Entity\Entity.h>
#include <Projectile\Projectile.h>

namespace Logic
{
	class Player;
	class Physics;
	class GrapplingPoint : public Entity
	{
	public:
		GrapplingPoint(btRigidBody* body, btVector3 halfExtent, Graphics::ModelID modelID = Graphics::SPHERE);
		~GrapplingPoint();

		void init(Physics* physics, Player* player);
		void onCollision(Entity& other);
		
	private:
		Physics* m_physicsPtr;  //< We need physics because we need to perform a raytest 
		Player* m_playerPtr;

	};

}

#endif // !GRAPPLINGPOINT_H
