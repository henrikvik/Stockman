#ifndef MAP_H
#define MAP_H

#include <vector>
#include <Entity\Object.h>
#include <Physics\Physics.h>
#include <Entity\GrapplingPoint.h>

namespace Logic
{
	class Map
	{
	public:
		Map();
		Map(const Map& other) = delete;
		Map* operator=(const Map& other) = delete;
		~Map();

		void init(Physics* physics, Player* player);
		void clear();
		void update(float deltaTime);
		void render(Graphics::Renderer& renderer);

		std::vector<Object*>*			getProps();
		std::vector<Entity*>*			getHitboxes();
		std::vector<Entity*>*			getObjects();
		std::vector<GrapplingPoint*>*	getGrapplingPoints();

	private:
		std::vector<Object*>			m_props;
		std::vector<Entity*>			m_hitboxes;
		std::vector<Entity*>			m_objects;
		std::vector<GrapplingPoint*>	m_grapplingPoints;

		bool m_drawHitboxes;	// debugging purposes

		void initProps();
		void initHitboxes(Physics* physics);
		void initObjects(Physics* physics);
		void initGrapplingPoints(Physics* physics, Player* player);
	};
}


#endif // !MAP_H
