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
	btBroadphaseInterface *broadphase = new btDbvtBroadphase();
	btDefaultCollisionConfiguration *collision_config = new btDefaultCollisionConfiguration();
	btCollisionDispatcher *dispatcher = new btCollisionDispatcher(collision_config);
	btSequentialImpulseConstraintSolver *solver = new btSequentialImpulseConstraintSolver;
	btDiscreteDynamicsWorld *world =
		new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collision_config);

	world->setGravity(btVector3(0.0f, -9.81f, 0.0f));
	world->addRigidBody(ground_rigid_body);

	while (!main_window.should_close()) {
		main_window.process_input();

		world->stepSimulation(1.0f / 60.0f, 10);

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
		main_window.translate_model(0.0f, -1.0f, 0.0f);
		main_window.scale_model(10.0f);
		ground_shader.set_mat4("model", main_window.model);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		//==== model
		model_shader.use();

		main_window.reset_model();
		main_window.translate_model(-3.0f, 1.0f, 0.0f);
		main_window.scale_model(0.1f);
		nanosuit.draw(main_window, model_shader);

		main_window.reset_model();
		main_window.translate_model(3.0f, 1.0f, 3.0f);
		main_window.scale_model(0.3f);
		oldtimer.draw(main_window, model_shader);

		// poll current events and swap the buffers
		glfwPollEvents();
		main_window.swap_buffers();
	}

	delete ground_rigid_body;

	delete world;
	delete solver;
	delete dispatcher;
	delete collision_config;
	delete broadphase;

	glfwTerminate();
}
