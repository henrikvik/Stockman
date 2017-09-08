#ifndef MAP_H
#define MAP_H

#include <vector>
#include "Entity/Entity.h"

namespace Logic
{
	class Map
	{
	public:
		Map();
		Map(const Map& other) = delete;
		Map* operator=(const Map& other) = delete;
		~Map();

		void clear();
		void addProp();

		std::vector<Object*>* getProps();
	//	std::vector<Hitbox*>* getHitboxes();

	private:
		std::vector<Object*> props;
	//	std::vector<Hitbox*> hitboxes;
	};
}


#endif // !MAP_H
