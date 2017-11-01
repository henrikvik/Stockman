#ifndef MAP_H
#define MAP_H

#include <vector>
#include <Entity\Object.h>
#include <Physics\Physics.h>
#include <Entity\Entity.h>
#include <Entity/StaticObject.h>
#include <Misc\MapStructs.h>
#include <Entity/LightObject.h>
#include <Misc\FileLoader.h>

namespace Logic
{
	class Map
	{
	public:
		Map();
		Map(const Map& other) = delete;
		Map* operator=(const Map& other) = delete;
		~Map();

        void add(FrameLight frameLight);
        void add(FrameProp frameProp);
        void add(FrameHitbox frameHitbox);

		void init(Physics* physics);
		void clear();
		void update(float deltaTime);
		void render();

		std::vector<StaticObject*>*			getProps();
		std::vector<StaticObject*>*			getHitboxes();
		std::vector<Speaker*>*				getObjects();
        std::vector<LightObject*>*			getLights();

	private:
        std::vector<StaticObject*>          m_props;
		std::vector<StaticObject*>			m_hitboxes;
		std::vector<Speaker*>				m_objects;
        std::vector<LightObject*>			m_lights;

        Physics* m_physicsPtr;

        void readFromFile(std::string path);

        // DEBUG & TESTING STUFF
        bool m_drawHitboxesAndLights;
		void debugInitProps();          // move over to readfromfile
		void debugInitObjects();        // move over to readfromfile
	};
}


#endif // !MAP_H
