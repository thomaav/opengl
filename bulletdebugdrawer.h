#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <btBulletDynamicsCommon.h>

// https://github.com/opengl-tutorials/ogl/blob/master/misc05_picking/misc05_picking_BulletPhysics.cpp

class BulletDebugDrawer : public btIDebugDraw {
public:
	int m;
	void BulletSetMatrices(glm::mat4 pViewMatrix, glm::mat4 pProjectionMatrix);
	void SetMatrices(glm::mat4 pViewMatrix, glm::mat4 pProjectionMatrix);
	void drawLine(const btVector3& from,const btVector3& to,const btVector3& color) override;
	void drawContactPoint(const btVector3 &, const btVector3 &, btScalar, int, const btVector3 &) override { }
	void reportErrorWarning(const char *) override { }
	void draw3dText(const btVector3 &,const char *) override { }
	void setDebugMode(int p) override { m = p; }
	int getDebugMode(void) const override { return 3; }
};
