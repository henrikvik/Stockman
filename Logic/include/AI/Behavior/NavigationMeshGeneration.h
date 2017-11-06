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
    // Based on PASVF described in the paper
    // "A growth-based approach to the automatic
    // generation of navigation meshes" by
    //                      Hale, David Hunter, Ph.D.
    // 
    // This algorithm uses bullet physics for collision.
    // For more information see my book "PASVF with Bullet" [3].
    //
    // [1]: https://github.com/henrikvik/DV1544-Stort-Spel/issues
    // [2]: https://pqdtopen.proquest.com/doc/920011245.html?FMT=ABS
    // [3]: https://goo.gl/dzpBc

    class Physics;
    class StaticObject;
	class NavigationMeshGeneration 
	{
        private: //var
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
                X_PLUS, Z_PLUS, X_MINUS, Z_MINUS
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
                NavMeshCube *buddy;
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
            void split(std::vector<NavMeshCube> &regions, Physics &physics, NavMeshCube &cube,
                btVector3 const &cubeColPoint, btVector3 const &splitPlaneNormal);
            void removeRigidBody(btRigidBody *body, Physics &physics);

            // true on collision
            std::pair<bool, btVector3> NavigationMeshGeneration::rayTestCollisionPoint(StaticObject *obj, btRigidBody *reg, Physics &physics, btVector3 &normalIncrease, float maxDistance);
	};
}

#endif