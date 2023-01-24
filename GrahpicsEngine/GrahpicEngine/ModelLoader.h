#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <unordered_map>
#include <memory>

class Mesh;

class ModelLoader
{
public:
	ModelLoader();

	void Load(std::string name, std::string fileName);
	Mesh* Get(std::string name);

private:
	bool Completed(const aiScene* scene);
	void CreateNode(aiNode* node, const aiScene* scene);
	void CreateMesh(aiMesh* mesh, const aiScene* scene);

	Assimp::Importer import;
	const std::string rootPath = "../models/";
	bool IsExist(std::string name);

	std::vector<glm::vec3> vertex;
	std::vector<glm::vec3> normal;
	std::vector<glm::vec2> textureCoordinate;
	std::vector<unsigned int> indices;

	std::unordered_map<std::string, std::unique_ptr<Mesh>> models;
};

extern ModelLoader* MODELLOAD;