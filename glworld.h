#pragma once

#include <btBulletDynamicsCommon.h>

class World {
public:
	btDiscreteDynamicsWorld *physics_world;

	World();
	~World();

	World(const World &o) = delete;
	World & operator=(const World &o) = delete;
	World(World &&o) = delete;

	void set_gravity(float x, float y, float z);
private:
	btBroadphaseInterface *broadphase;
	btDefaultCollisionConfiguration *collision_config;
	btCollisionDispatcher *dispatcher;
	btSequentialImpulseConstraintSolver *solver;
};
