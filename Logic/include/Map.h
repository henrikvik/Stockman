#ifndef MAP_H
#define MAP_H

#include <vector>
#include <Entity\Object.h>
#include <Physics\Physics.h>

namespace Logic
{
	class Map
	{
	public:
		Map();
		Map(const Map& other) = delete;
		Map* operator=(const Map& other) = delete;
		~Map();

		void init(Physics* physics);
		void clear();
		void update(float deltaTime);
		void render(RenderRegister& renderRegister);

		std::vector<Object*>* getProps();
		std::vector<Entity*>* getHitboxes();
		std::vector<Entity*>* getObjects();

	private:
		std::vector<Object*> m_props;
		std::vector<Entity*> m_hitboxes;
		std::vector<Entity*> m_objects;
		bool m_drawHitboxes;	// debugging purposes

		void initProps();
		void initHitboxes(Physics* physics);
		void initObjects(Physics* physics);
	};
}


#endif // !MAP_H
