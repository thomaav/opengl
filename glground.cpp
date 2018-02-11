#include "glground.h"

float ground[30] = {
	-1.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	1.0f, 0.0f, 1.0f,   30.0f, 0.0f,
	1.0f, 0.0f, -1.0f,  30.0f, 30.0f,

	-1.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	-1.0f, 0.0f, -1.0f, 0.0f, 30.0f,
	1.0f, 0.0f, -1.0f,  30.0f, 30.0f,
};

GLuint gl_init_ground()
{
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground), ground, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	return VAO;
}

btCollisionShape *shape;
btDefaultMotionState *motion_state;
btRigidBody *rigid_body;

btRigidBody *init_ground_physics()
{
	shape = new btStaticPlaneShape(btVector3(0.0f, 1.0f, 0.0f), 1.0f);
	motion_state =
		new btDefaultMotionState(btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f),
						     btVector3(0.0f, -1.0f, 0.0f)));
	btRigidBody::btRigidBodyConstructionInfo
		rigid_body_info(0, motion_state, shape, btVector3(0.0f, 0.0f, 0.0f));
	rigid_body = new btRigidBody(rigid_body_info);

	// delete shape;
	// delete motion_state;

	return rigid_body;
}
