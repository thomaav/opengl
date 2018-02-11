#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <btBulletDynamicsCommon.h>

#include "glshader.h"
#include "gltexture.h"
#include "glwindow.h"
#include "glmaterial.h"
#include "gllight.h"
#include "glmesh.h"
#include "glmodel.h"
#include "glground.h"
#include "glworld.h"

class BulletDebugDrawer_DeprecatedOpenGL : public btIDebugDraw{
public:
	void SetMatrices(glm::mat4 pViewMatrix, glm::mat4 pProjectionMatrix){
		glUseProgram(0); // Use Fixed-function pipeline (no shaders)
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(&pViewMatrix[0][0]);
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(&pProjectionMatrix[0][0]);
	}
	virtual void drawLine(const btVector3& from,const btVector3& to,const btVector3& color){
		glColor3f(color.x(), color.y(), color.z());
		glBegin(GL_LINES);
			glVertex3f(from.x(), from.y(), from.z());
			glVertex3f(to.x(), to.y(), to.z());
		glEnd();
	}
	virtual void drawContactPoint(const btVector3 &,const btVector3 &,btScalar,int,const btVector3 &){}
	virtual void reportErrorWarning(const char *){}
	virtual void draw3dText(const btVector3 &,const char *){}
	virtual void setDebugMode(int p){
		m = p;
	}
	int getDebugMode(void) const {return 3;}
	int m;
};

int main(int argc, char *argv[])
{
	Window main_window{false};

	Shader texture_shader{"shaders/texture_vs.glsl", "shaders/texture_fs.glsl"};
	Shader lighting_shader{"shaders/lighting_vs.glsl", "shaders/lighting_fs.glsl"};
	Shader ground_shader{"shaders/ground_vs.glsl", "shaders/ground_fs.glsl"};
	Shader mesh_shader{"shaders/mesh_vs.glsl", "shaders/mesh_fs.glsl"};
	Shader model_shader{"shaders/model_vs.glsl", "shaders/model_fs.glsl"};
	Texture ground_texture{"textures/grass.png"};

	Model nanosuit{"models/nanosuit/nanosuit.obj"};
	Model oldtimer{"models/oldtimer/Old_timer.obj"};

	GLuint ground_VAO = gl_init_ground();
	btRigidBody *ground_rigid_body = init_ground_physics();

	//==== physics
	World world{};

	world.set_gravity(0.0f, -9.81f, 0.0f);
	world.physics_world->addRigidBody(ground_rigid_body);
	world.physics_world->addRigidBody(nanosuit.rigid_body);
	world.physics_world->addRigidBody(oldtimer.rigid_body);

	while (!main_window.should_close()) {
		main_window.process_input();

		world.physics_world->stepSimulation(1.0f / 60.0f, 10);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//==== ground
		glBindTexture(GL_TEXTURE_2D, 0);

		ground_shader.use();
		ground_shader.set_mat4("view", main_window.view);
		ground_shader.set_mat4("projection", main_window.projection);

		glBindVertexArray(ground_VAO);

		ground_shader.set_int("ground_texture", 0);
		ground_texture.use(GL_TEXTURE0);

		ground_shader.set_vec3("light_color", light.ambient);
		ground_shader.set_vec3("object_color", glm::vec3{0.75f, 0.75f, 0.75f});

		main_window.reset_model();
		main_window.scale_model(10.0f);
		ground_shader.set_mat4("model", main_window.model);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		//==== model
		model_shader.use();

		main_window.reset_model();
		main_window.translate_model(nanosuit.get_transform().getOrigin().getX(),
					    nanosuit.get_transform().getOrigin().getY(),
					    nanosuit.get_transform().getOrigin().getZ());
		main_window.scale_model(0.1f);
		nanosuit.draw(main_window, model_shader);

		main_window.reset_model();
		main_window.translate_model(oldtimer.get_transform().getOrigin().getX() + 3.0f,
					    oldtimer.get_transform().getOrigin().getY(),
					    oldtimer.get_transform().getOrigin().getZ() + 3.0f);
		main_window.scale_model(0.1f);
		oldtimer.draw(main_window, model_shader);

		// poll current events and swap the buffers
		glfwPollEvents();
		main_window.swap_buffers();
	}

	delete ground_rigid_body;

	glfwTerminate();
}
