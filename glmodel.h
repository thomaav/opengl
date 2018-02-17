#pragma once

#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glshader.h"
#include "glmesh.h"
#include "glwindow.h"
#include "gltexture.h"

class Model {
public:
	btRigidBody *rigid_body;

	glm::mat4 model;

	Model(std::string path);
	~Model();

	Model(const Model &o) = delete;
	Model & operator=(const Model &o) = delete;

	void init_physics();
	btTransform get_transform();

	void reset_model();

	void translate_model(float x, float y, float z);
	void translate_model(glm::vec3 translation);

	void rotate_model(float radians, float x, float y, float z);
	void rotate_model(float radians, glm::vec3 rotation);

	void scale_model(float x, float y, float z);
	void scale_model(float scalar);

	void move_model(float x, float y, float z);

	void draw(Window &window, Shader &shader);
private:
	std::vector<Mesh> meshes;
	std::string dir;
	std::vector<std::string> texture_fps;

	float mass;
	btCollisionShape *shape;
	btDefaultMotionState *motion_state;
	btVector3 inertia;

	void load_model(std::string path);
	void process_node(aiNode *node, const aiScene *scene);
	Mesh process_mesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> load_material_textures(aiMaterial *mat, aiTextureType type, std::string type_name);
};
