#pragma once

#include <glad/glad.h>
#include <btBulletDynamicsCommon.h>

extern float ground[30];

extern btCollisionShape *shape;
extern btDefaultMotionState *motion_state;
extern btRigidBody *rigid_body;

GLuint gl_init_ground();
btRigidBody *init_ground_physics();
