/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ModelLoader.h
Purpose: Making of header file for ModelLoader System
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <string>
#include <unordered_map>
#include <memory>
#include "ModelTransformFixer.h"

class ModelMesh;

class ModelLoader
{
public:
	ModelLoader();
	~ModelLoader();

	void Load(std::string name, std::string fileName);
	ModelMesh* Get(std::string name);

	const std::string rootPath = "models/";
private:
	bool Completed(const aiScene* scene);
	void CreateNode(aiNode* node, const aiScene* scene);
	void CreateMesh(aiMesh* mesh, const aiScene* scene);

	Assimp::Importer import;
	bool IsExist(std::string name);

	FixerData data;
	std::vector<glm::vec3> vertex;
	std::vector<glm::vec3> vertexNormal;
	std::vector<glm::vec2> textureCoordinate;
	std::vector<unsigned int> indices;

	std::unordered_map<std::string, std::unique_ptr<ModelMesh>> models;
};

extern ModelLoader* MODELLOAD;