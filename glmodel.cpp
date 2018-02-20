#include <iostream>

#include "glmodel.h"

Shader* Model::stencil_shader = nullptr;

void Model::use_stencil(Shader *stencil_shader)
{
	this->stencil_shader = stencil_shader;
	outline = true;
}

void Model::disable_stencil()
{
	stencil_shader = nullptr;
	outline = false;
}

Model::Model(std::string path)
	: outline(false)
{
	model = glm::mat4{};

	load_model(path);
	init_physics();
}

Model::~Model()
{
	if (rigid_body)
		delete rigid_body;

	if (motion_state)
		delete motion_state;

	if (shape)
		delete shape;
}

void Model::init_physics()
{
	mass = 1.0f;
	inertia = btVector3(0.0f, 0.0f, 0.0f);

	shape = new btBoxShape(btVector3(1.0f, 1.0, 1.0f));
	motion_state = new btDefaultMotionState(btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f),
							    btVector3(0.0f, 3.0f, 0.0f)));
	shape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo
		rigid_body_info(mass, motion_state, shape, inertia);
	rigid_body = new btRigidBody(rigid_body_info);
}

btTransform Model::get_transform()
{
	return rigid_body->getWorldTransform();
}

void Model::reset_model()
{
	for (auto &mesh : meshes)
		mesh.reset_model();
}

void Model::translate_model(float x, float y, float z)
{
	for (auto &mesh : meshes)
		mesh.translate_model(x, y, z);
}

void Model::translate_model(glm::vec3 translation)
{
	for (auto &mesh : meshes)
		mesh.translate_model(translation);
}

void Model::rotate_model(float radians, float x, float y, float z)
{
	for (auto &mesh : meshes)
		mesh.rotate_model(radians, x, y, z);
}

void Model::rotate_model(float radians, glm::vec3 rotation)
{
	for (auto &mesh : meshes)
		mesh.rotate_model(radians, rotation);
}

void Model::scale_model(float x, float y, float z)
{
	for (auto &mesh : meshes)
		mesh.scale_model(x, y, z);
}

void Model::scale_model(float scalar)
{
	for (auto &mesh : meshes)
		mesh.scale_model(scalar);
}

void Model::move_model(float x, float y, float z)
{
	for (auto &mesh : meshes)
		mesh.move_model(x, y, z);
}

void Model::draw(Window &window, Shader &shader)
{
	if (stencil_shader && outline) {
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
	} else {
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0x00);
	}

	for (unsigned i = 0; i < meshes.size(); ++i) {
		meshes[i].draw(window, shader);
	}

	if (stencil_shader && outline) {
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glDisable(GL_DEPTH_TEST);
		stencil_shader->use();

		for (unsigned i = 0; i < meshes.size(); ++i) {
			meshes[i].draw(window, *stencil_shader);
		}
	}

	glStencilMask(0x00);
	glEnable(GL_DEPTH_TEST);
}

void Model::load_model(std::string path)
{
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}

	dir = path.substr(0, path.find_last_of('/'));

	process_node(scene->mRootNode, scene);
}

void Model::process_node(aiNode *node, const aiScene *scene)
{
	for (unsigned i = 0; i < node->mNumMeshes; ++i) {
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(process_mesh(mesh, scene));
	}

	for (unsigned i = 0; i < node->mNumChildren; ++i)
		process_node(node->mChildren[i], scene);
}

Mesh Model::process_mesh(aiMesh *mesh, const aiScene *scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	for (unsigned i = 0; i < mesh->mNumVertices; ++i) {
		Vertex vertex;
		glm::vec3 tmp;

		tmp.x = mesh->mVertices[i].x;
		tmp.y = mesh->mVertices[i].y;
		tmp.z = mesh->mVertices[i].z;
		vertex.position = tmp;

		tmp.x = mesh->mNormals[i].x;
		tmp.y = mesh->mNormals[i].y;
		tmp.y = mesh->mNormals[i].y;
		vertex.normal = tmp;

		if (mesh->mTextureCoords[0]) {
			glm::vec2 tmp;

			tmp.x = mesh->mTextureCoords[0][i].x;
			tmp.y = mesh->mTextureCoords[0][i].y;
			vertex.texture_coords = tmp;
		} else {
			vertex.texture_coords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	for (unsigned i = 0; i < mesh->mNumFaces; ++i) {
		aiFace face = mesh->mFaces[i];
		for (unsigned j = 0; j < face.mNumIndices; ++j)
			indices.push_back(face.mIndices[j]);
	}

	aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

	// TODO: it's slow to resize the vector every time
	std::vector<Texture> diffuse_maps =
		load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	for (auto &tmp_texture : diffuse_maps)
		textures.push_back(std::move(tmp_texture));

	std::vector<Texture> specular_maps =
		load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
	for (auto &tmp_texture : specular_maps)
		textures.push_back(std::move(tmp_texture));

	return Mesh(vertices, indices, std::move(textures));
}

std::vector<Texture> Model::load_material_textures(aiMaterial *mat, aiTextureType type, std::string type_name)
{
	// TODO: skip already loaded textures
	std::vector<Texture> textures;

	for (unsigned i = 0; i < mat->GetTextureCount(type); ++i) {
		aiString str;
		mat->GetTexture(type, i, &str);
		Texture texture{dir + '/' + str.C_Str()};
		texture.type = type_name;
		textures.push_back(std::move(texture));
	}

	return textures;
}
