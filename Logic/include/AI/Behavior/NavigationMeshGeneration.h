#ifndef PASVF_H
#define PASVF_H

#include "NavigationMesh.h"

#include <d3d11.h>
#include <SimpleMath.h>
#include <Entity\Entity.h>
#include <cmath>
#include <Physics\Primitives.h>
#include <btBulletCollisionCommon.h>

#include <vector>
#define SIDES 4

namespace Logic
{
	//          Nav Mesh Gen v0.1
    // For errors in creating the navigation mesh
    // create a issue on github[1] or contact LW.
    //
    // Based on the NavMesh generation in series of
    // games, Tom Clancy's Ghost Recon[2].
    // 
    // [1]: https://github.com/henrikvik/DV1544-Stort-Spel/issues
    // [2]: http://store.steampowered.com/app/15300/Tom_Clancys_Ghost_Recon/

    class Physics;
    class StaticObject;
	class NavigationMeshGeneration 
	{
        private: //var
            float presicion;
		public:
			enum VertexOrder { CLOCKWISE, COUNTER_CLOCKWISE };

			struct Triangle {
				DirectX::SimpleMath::Vector3 normal; // normal will be loaded in generation
				DirectX::SimpleMath::Vector3 vertices[3]; // CW
			};

            struct NavStaticObject {
                std::vector<Triangle> tris;
            };

			NavigationMeshGeneration();
			virtual ~NavigationMeshGeneration();

            void registerGenerationCommand(NavigationMesh &nav,
                Physics &physics);

			void generateNavMeshOld(NavigationMesh &nav,
                std::vector<Triangle> terrain,
                std::vector<NavStaticObject> objects) const;
            void generateNavigationMesh(NavigationMesh &nav, Physics &physics);
		private:
            enum CollisionReturn
            {
                ON_AXIS, ON_EDGE, ON_VERTEX, PROBLEMS_MY_DUDES
            };
            struct Growth
            {
                btVector3 positionChange, dimensionChange; // half size
            };
            struct NavMeshCube
            {
                bool done, collided[SIDES];
                btRigidBody *body;
                Cube cube;

                NavMeshCube(Cube &cube)
                {
                    this->cube = cube;

                    done = false;
                    body = nullptr;

                    for (int i = 0; i < SIDES; i++)
                        collided[i] = false;
                }
            };

            DirectX::SimpleMath::Vector3 getNormal(
                Triangle const &triangle,
                VertexOrder vertexOrder = CLOCKWISE) const;

            std::pair<Triangle, Triangle> toTriangle(Cube &cube);
            NavigationMesh::Triangle toNavTriangle(Triangle const &tri);
            CollisionReturn handleCollision(btVector3 collisionPoint, NavMeshCube &cube, StaticObject *obj, Growth const &growth, btVector3 growthNormal, btBoxShape *shape);
            void split(std::vector<NavMeshCube> &region, NavMeshCube &cube, btVector3 cubeColPoint, int side);
            void removeRigidBody(btRigidBody *body, Physics &physics);

            // true on collision
            std::pair<bool, btVector3> NavigationMeshGeneration::rayTestCollisionPoint(StaticObject *obj, btRigidBody *reg, Physics &physics, btVector3 &normalIncrease, float maxDistance);

            // This is very nice
            class NavContactResult : public btCollisionWorld::ContactResultCallback {
                private:
                    std::function<bool(btBroadphaseProxy* proxy)> m_needsCollision;
                    std::function <btScalar(btManifoldPoint& cp,
                        const btCollisionObjectWrapper* colObj0, int partId0, int index0,
                        const btCollisionObjectWrapper* colObj1, int partId1, int index1)>
                        m_singleRes;
                public:
                    NavContactResult(
                        std::function<bool(btBroadphaseProxy* proxy)> needsCollision,
                        std::function <btScalar(btManifoldPoint& cp,
                            const btCollisionObjectWrapper* colObj0, int partId0, int index0,
                            const btCollisionObjectWrapper* colObj1, int partId1, int index1)> singleRes
                        ) {
                            m_needsCollision = needsCollision;
                            m_singleRes = singleRes;
                        }

                        virtual bool needsCollision(btBroadphaseProxy* proxy) const {
                            return m_needsCollision(proxy);
                        }

                        //! Called with each contact for your own processing (e.g. test if contacts fall in within sensor parameters)
                        virtual btScalar addSingleResult(btManifoldPoint& cp,
                            const btCollisionObjectWrapper* colObj0, int partId0, int index0,
                            const btCollisionObjectWrapper* colObj1, int partId1, int index1)
                        {
                            return m_singleRes(cp, colObj0, partId0, index0, colObj1, partId1, index1);
                        }
            };
	};
}

#endif