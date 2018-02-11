#pragma once

#include <glad/glad.h>
#include <btBulletDynamicsCommon.h>

extern float ground[30];

GLuint gl_init_ground();
btRigidBody *init_ground_physics();
