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
	Model(std::string path);
	void draw(Window &window, Shader &shader);
private:
	std::vector<Mesh> meshes;
	std::string dir;
	std::vector<std::string> texture_fps;

	void load_model(std::string path);
	void process_node(aiNode *node, const aiScene *scene);
	Mesh process_mesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> load_material_textures(aiMaterial *mat, aiTextureType type, std::string type_name);
};
