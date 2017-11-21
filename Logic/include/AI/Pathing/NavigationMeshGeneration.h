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
	//          Nav Mesh Gen v0.4
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
    // NP-Hard Problem

    /*
    
        THIS HAS TO BE HEAVILY REFACTORED, IT IS VERY BAD CODE AT THE MOMENT
        WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP
    
    */

    class Physics;
    class StaticObject;
	class NavigationMeshGeneration 
	{
        private: //var
            static const int AI_UID, NO_ID;
            static const float SEED_CUBES, PRECISION_BASE;
            static int COUNTER;
            static const btVector3 unitDimension;

            float precision, maxLength, baseY;
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
                bool done, collided[SIDES];
                int userIndex, buddyIndex, finishedIndex;
                std::vector<int> collidedWithIndex[SIDES]; // for the many, not the few

                btRigidBody *body;
                Cube cube;

                NavMeshCube(Cube &cube)
                {
                    this->cube = cube;

                    done = false;
                    body = nullptr;

                    buddyIndex = finishedIndex = -1;
                    loadIndex();

                    for (int i = 0; i < SIDES; i++)
                        collided[i] = false;
                }

                inline void addCollision(int side, int buddyId)
                {
                    collidedWithIndex[side].push_back(buddyId);
                }

                inline int hasCollision(int buddyId)
                {
                    for (int side = 0; side < SIDES; side++)
                    {
                        for (int index : collidedWithIndex[side])
                            if (index == buddyId)
                            {
                                return side;
                            }
                    }
                    return -1;
                }

                inline void loadIndex()
                {
                    userIndex = COUNTER++;
                }
            };

            std::vector<NavMeshCube> regions;
            std::vector<int> regionsFinished;
            std::vector<std::pair<btCollisionObject*, StaticObject*>> physicsObjects;

            Growth growth[SIDES];
            btVector3 growthNormals[SIDES];

            DirectX::SimpleMath::Vector3 getNormal(
                Triangle const &triangle,
                VertexOrder vertexOrder = CLOCKWISE) const;

            std::pair<Triangle, Triangle> toTriangle(Cube &cube);
            // cube1 = 
            std::pair<Cube, Cube> cutCube(btVector3 const &cutPoint, btVector3 const &planeNormal, Cube const &cube);

            // returns true if collision happend
            void loadPhysicsObjects(Physics &physics);
            // returns true if collision happend
            bool handlePhysicsCollisionTest(NavMeshCube &region, Physics &physics, int side);
            bool handleRegionCollisionTest(NavMeshCube &region, Physics &physics, int side);

            void quadMeshToTriangleMesh(NavigationMesh &nav, Physics &physics);
            void growRegion(NavMeshCube &cube, Growth const &growth);
            void shrinkRegion(NavMeshCube &cube, Growth const &growth);

            int getRegion(int id) const;
            btVector3 getDimension(NavMeshCube &region, int side) const;

            NavigationMesh::Triangle toNavTriangle(Triangle const &tri);
            CollisionReturn handleCollision(btVector3 collisionPoint, NavMeshCube &cube,
                StaticObject *obj, Growth const &growth, btVector3 growthNormal, btBoxShape *shape);

            void split(NavMeshCube &cube, Physics &physics, btVector3 const &cubeColPoint,
                btVector3 const &splitPlaneNormal);
            void removeRigidBody(btRigidBody *&body);

            // if a cube is split to two cubes, then you can have collision to "both" sides,
            // so add secondIndex if you have collided with first index
            void addSplitIndices(int firstIndex, int secondIndex);

            bool isInCollisionArea(NavMeshCube &cube, Physics &physics, int filterId = NO_ID, int filterId1 = NO_ID);
            void seedArea(btVector3 position, btVector3 fullDimension, float part, Physics &physics);

            // create edges
            void createEdgeBeetwen(NavigationMesh &nav, int r1, int r2, int triangle1, int triangle2, GrowthType side);

            // true on collision
            std::pair<bool, btVector3> NavigationMeshGeneration::rayTestCollisionPoint(StaticObject *obj, btRigidBody *reg, Physics &physics, btVector3 &normalIncrease, float maxDistance);

            // For creating test data
            void testFunc(Physics &physics);
	};
}

#endif