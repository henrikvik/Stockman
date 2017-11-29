#ifndef MAP_H
#define MAP_H

#pragma region Comment

/*

    Description: Container class that carries all non-interactable things on the map
                    Examples would be: Hitboxes, Visual Models, etc.

                 Classes in this container is not updatable and do not change.

                 Should not be operatated from the outside. 
                 Only from file.

*/

#pragma endregion Description of class

// Includes
#include <vector>
#include <Misc\NonCopyable.h>
#include <Entity/StaticObject.h>
#include <Entity/LightObject.h>
#include <Misc\MapStructs.h>

namespace Logic
{
    class Decoration
    {
    public:
        Decoration(Resources::Models::Files model, DirectX::SimpleMath::Matrix transform, float cull_radius)
        {
            renderInfo.model = model;
            renderInfo.transform = transform;
            renderInfo.color = DirectX::SimpleMath::Vector3( 1, 1, 1);
            renderInfo.useGridTexture = false;
            renderInfo.cull_radius = cull_radius;
        }

        void render() const
        {
            QueueRender(renderInfo);
        }
    private:
        StaticRenderInfo renderInfo;
    };

    class StaticObject;
    class Physics;
	class Map : public NonCopyable
	{
	public:
		Map();
        ~Map();

		void init(Physics* physics);
		void clear();
		void update(float deltaTime);
		void render() const;

        void loadStartMenuScene();
        void loadMap(Resources::Maps::Files map);

		std::vector<StaticObject*>*			getProps();
		std::vector<StaticObject*>*			getHitboxes();
        std::vector<LightObject*>*			getLights();

	private:
        std::vector<Decoration>     decorations;
		std::vector<StaticObject*>			m_props;        //< Visual objects
		std::vector<StaticObject*>			m_hitboxes;     //< Non-visual hitboxes
        std::vector<LightObject*>			m_lights;       //< Pointlights
        bool                                m_drawDebug;    //< Toggle for debug-draw
        Physics*                            m_physicsPtr;   //< Ptr to physics for creation of hitboxes

        // Private functions
        void add(FrameLight frameLight);
        void add(FrameProp frameProp);
        void add(FrameHitbox frameHitbox);
	};
}

#endif // !MAP_H