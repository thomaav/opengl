#include <iostream>

#include "glmodel.h"

Model::Model(std::string path)
{
	load_model(path);
}

void Model::draw(Window &window, Shader &shader)
{
	for (unsigned i = 0; i < meshes.size(); ++i) {
		meshes[i].draw(window, shader);
	}
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
		Texture texture{dir + '/' + str.C_Str(), true};
		texture.type = type_name;
		textures.push_back(std::move(texture));
	}

	return textures;
}
