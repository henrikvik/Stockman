#ifndef MAP_H
#define MAP_H

#include <vector>
#include <Entity\Object.h>
#include <Entity\Hitbox.h>
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
		void render(RenderRegister& renderRegister);

		std::vector<Object*>* getProps();
		std::vector<Hitbox*>* getHitboxes();

	private:
		std::vector<Object*> m_props;
		std::vector<Hitbox*> m_hitboxes;
		bool m_drawHitboxes;	// debugging purposes

		void initProps();
		void initHitboxes(Physics* physics);
	};
}


#endif // !MAP_H
