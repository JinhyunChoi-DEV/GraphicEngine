#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <string>
#include <unordered_map>
#include <memory>
#include "ModelTransformFixer.h"

class Mesh;

class ModelLoader
{
public:
	ModelLoader();
	~ModelLoader();

	void Load(std::string name, std::string fileName);
	Mesh* Get(std::string name);

private:
	bool Completed(const aiScene* scene);
	void CreateNode(aiNode* node, const aiScene* scene);
	void CreateMesh(aiMesh* mesh, const aiScene* scene);

	Assimp::Importer import;
	const std::string rootPath = "models/";
	bool IsExist(std::string name);

	FixerData data;
	std::vector<glm::vec3> vertex;
	std::vector<glm::vec3> vertexNormal;
	std::vector<glm::vec3> faceNormal;
	std::vector<glm::vec2> textureCoordinate;
	std::vector<unsigned int> indices;

	std::unordered_map<std::string, std::unique_ptr<Mesh>> models;
};

extern ModelLoader* MODELLOAD;