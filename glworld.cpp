#include <iostream>

#include "glworld.h"

World::World()
{
	broadphase = new btDbvtBroadphase();
	collision_config = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collision_config);
	solver = new btSequentialImpulseConstraintSolver;
	physics_world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collision_config);
}

World::~World()
{
	if (physics_world)
		delete physics_world;

	if (solver)
		delete solver;

	if (dispatcher)
		delete dispatcher;

	if (collision_config)
		delete collision_config;

	if (broadphase)
		delete broadphase;
}

void World::set_gravity(float x, float y, float z)
{
	physics_world->setGravity(btVector3(x, y, z));
}
