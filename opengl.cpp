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
#include "bulletdebugdrawer.h"

Model *nanosuit_ptr;

int main(int argc, char *argv[])
{
	Window main_window{false};

	Shader texture_shader{"shaders/texture_vs.glsl", "shaders/texture_fs.glsl"};
	Shader lighting_shader{"shaders/lighting_vs.glsl", "shaders/lighting_fs.glsl"};
	Shader ground_shader{"shaders/ground_vs.glsl", "shaders/ground_fs.glsl"};
	Shader mesh_shader{"shaders/mesh_vs.glsl", "shaders/mesh_fs.glsl"};
	Shader model_shader{"shaders/model_vs.glsl", "shaders/model_fs.glsl"};
	Texture ground_texture{"textures/grass.png"};

	model_shader.add_light(point_light);
	model_shader.add_light(dir_light);
	ground_shader.add_light(point_light);

	Model nanosuit{"models/nanosuit/nanosuit.obj"};
	nanosuit_ptr = &nanosuit;
	nanosuit.scale_model(0.1f);

	Model nanosuit_right{"models/nanosuit/nanosuit.obj"};
	Model nanosuit_left{"models/nanosuit/nanosuit.obj"};

	GLuint ground_VAO = gl_init_ground();
	btRigidBody *ground_rigid_body = init_ground_physics();
	glm::mat4 ground_model = glm::mat4{};

	//==== physics
	World world{};

	world.set_gravity(0.0f, -9.81f, 0.0f);
	world.physics_world->addRigidBody(ground_rigid_body);
	world.physics_world->addRigidBody(nanosuit.rigid_body);
	// world.physics_world->addRigidBody(nanosuit_left.rigid_body);
	// world.physics_world->addRigidBody(nanosuit_right.rigid_body);

	BulletDebugDrawer debug_drawer;
	world.physics_world->setDebugDrawer(&debug_drawer);

	while (!main_window.should_close()) {
		main_window.process_input();

		world.physics_world->stepSimulation(1.0f / 60.0f, 10);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//==== debug view for bullet
		debug_drawer.SetMatrices(main_window.view, main_window.projection);
		world.physics_world->debugDrawWorld();

		//==== ground
		glBindTexture(GL_TEXTURE_2D, 0);

		ground_shader.use();
		ground_shader.set_mat4("view", main_window.view);
		ground_shader.set_mat4("projection", main_window.projection);

		glBindVertexArray(ground_VAO);

		ground_shader.set_int("ground_texture", 0);
		ground_texture.use(GL_TEXTURE0);

		ground_shader.set_vec3("light_color", point_light->ambient);
		ground_shader.set_vec3("object_color", glm::vec3{0.75f, 0.75f, 0.75f});

		ground_model = glm::mat4{};
		ground_model = glm::scale(ground_model, glm::vec3(10.0f, 10.0f, 10.0f));
		ground_shader.set_mat4("model", ground_model);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		//==== model
		model_shader.use();

		nanosuit.reset_model();
		nanosuit.translate_model(nanosuit.get_transform().getOrigin().getX(),
					 nanosuit.get_transform().getOrigin().getY() - 1.0f,
					 nanosuit.get_transform().getOrigin().getZ() - 1.0f);
		nanosuit.scale_model(0.1f);
		nanosuit.draw(main_window, model_shader);

		nanosuit.reset_model();
		nanosuit.translate_model(nanosuit.get_transform().getOrigin().getX() - 1.0f,
					 nanosuit.get_transform().getOrigin().getY() - 1.0f,
					 nanosuit.get_transform().getOrigin().getZ());
		nanosuit.scale_model(0.1f);
		nanosuit.draw(main_window, model_shader);

		nanosuit.reset_model();
		nanosuit.translate_model(nanosuit.get_transform().getOrigin().getX() + 1.0f,
					 nanosuit.get_transform().getOrigin().getY() - 1.0f,
					 nanosuit.get_transform().getOrigin().getZ());
		nanosuit.scale_model(0.1f);
		nanosuit.draw(main_window, model_shader);

		nanosuit.reset_model();
		nanosuit.translate_model(nanosuit.get_transform().getOrigin().getX() + 25.0f,
					 nanosuit.get_transform().getOrigin().getY() - 1.0f,
					 nanosuit.get_transform().getOrigin().getZ());
		nanosuit.scale_model(0.1f);
		nanosuit.draw(main_window, model_shader);

		nanosuit.reset_model();
		nanosuit.translate_model(nanosuit.get_transform().getOrigin().getX() + 25.0f,
					 nanosuit.get_transform().getOrigin().getY() - 1.0f,
					 nanosuit.get_transform().getOrigin().getZ());
		nanosuit.scale_model(0.1f);
		nanosuit.draw(main_window, model_shader);

		// poll current events and swap the buffers
		glfwPollEvents();
		main_window.swap_buffers();
	}

	delete ground_rigid_body;

	glfwTerminate();
}
