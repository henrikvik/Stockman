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
	//          Nav Mesh Gen v0.3
    // For errors in creating the navigation mesh
    // create a issue on github[1] or contact LW.
    //
    // Based on PASVF described in the paper
    // "A growth-based approach to the automatic
    // generation of navigation meshes"[2] by
    //                      Hale, David Hunter, Ph.D.
    // 
    // This algorithm uses bullet physics for collision.
    // For more information see my book "PASVF with Bullet" [3].
    //
    // [1]: https://github.com/henrikvik/DV1544-Stort-Spel/issues
    // [2]: https://pqdtopen.proquest.com/doc/920011245.html?FMT=ABS
    // [2] (Mirror): https://libres.uncg.edu/ir/uncc/f/Hale_uncc_0694D_10272.pdf
    // [3]: https://goo.gl/dzpBc
    // (Not ready for production)

    class Physics;
    class StaticObject;
	class NavigationMeshGeneration 
	{
        private: //var
            float precision, maxLength, baseY;
            static const int AI_UID, NO_ID;
            static int COUNTER;
            static const btVector3 unitDimension;
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

            void registerSetCommands();
            void registerGenerationCommand(NavigationMesh &nav,
                Physics &physics);

			void generateNavMeshOld(NavigationMesh &nav,
                std::vector<Triangle> terrain,
                std::vector<NavStaticObject> objects) const;
            void generateNavigationMesh(NavigationMesh &nav, Physics &physics);
		private:
            enum GrowthType // Clockwise
            {
                X_PLUS, Z_MINUS, X_MINUS, Z_PLUS
            };
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
                bool done, remove, collided[SIDES];
                int userIndex, buddyIndex;
                btRigidBody *body;
                Cube cube;

                NavMeshCube(Cube &cube)
                {
                    this->cube = cube;

                    done = remove = false;
                    body = nullptr;

                    buddyIndex = -1;
                    loadIndex();

                    for (int i = 0; i < SIDES; i++)
                        collided[i] = false;
                }

                inline void loadIndex()
                {
                    userIndex = COUNTER++;
                }
            };
            Growth growth[SIDES];
            btVector3 growthNormals[SIDES];

            DirectX::SimpleMath::Vector3 getNormal(
                Triangle const &triangle,
                VertexOrder vertexOrder = CLOCKWISE) const;

            std::pair<Triangle, Triangle> toTriangle(Cube &cube);
            std::pair<Cube, Cube> cutCube(btVector3 const &cutPoint, btVector3 const &planeNormal, Cube const &cube);

            NavigationMesh::Triangle toNavTriangle(Triangle const &tri);
            CollisionReturn handleCollision(btVector3 collisionPoint, NavMeshCube &cube,
                StaticObject *obj, Growth const &growth, btVector3 growthNormal, btBoxShape *shape);

            void split(std::vector<NavMeshCube> &regions, NavMeshCube &cube,
                Physics &physics, btVector3 const &cubeColPoint, btVector3 const &splitPlaneNormal);
            void removeRigidBody(btRigidBody *&body, Physics &physics);

            bool isInCollisionArea(NavMeshCube &cube, Physics &physics, int filterId = NO_ID, int filterId1 = NO_ID);
            void seedArea(btVector3 position, btVector3 fullDimension, float part, std::vector<NavMeshCube> &regions, Physics &physics);

            // true on collision
            std::pair<bool, btVector3> NavigationMeshGeneration::rayTestCollisionPoint(StaticObject *obj, btRigidBody *reg, Physics &physics, btVector3 &normalIncrease, float maxDistance);
	};
}

#endif