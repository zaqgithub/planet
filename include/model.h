#pragma once
#include "precompile.h"
#include "ShaderClass.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoords;
	glm::vec3 tangent;
	glm::vec3 bitangent;
	int m_BoneIDs[4];
	float m_Weights[4];
};

struct Texture {
	unsigned int id;
	std::string path;
	std::string type;
};

class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<Texture> textures;
	std::vector<unsigned int> indices;

	Mesh(const std::vector<Vertex>& vertices,const std::vector<Texture>& textures,const std::vector<unsigned int>& indices);
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,const std::vector<Texture>& textures);
	void draw(Shader& shader);
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};

class Model {

public:
	Model(std::string&& path) {
		loadModel(std::move(path));
	}
	Model(std::string& path) {
		loadModel(path);
	}

	void draw(Shader& shader);

	std::vector<Mesh> meshes; // restore the meshes 

	std::string directory;//record the current path of the obj file

	std::vector<Texture> textures_loaded;// stores all the textures loaded so far 

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture>  loadMaterialTextures(aiMaterial* mat, aiTextureType type,const std::string& typeName);
	unsigned int TextureFromFile(const char* path, const std::string& directory);
};