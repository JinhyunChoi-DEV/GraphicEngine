/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ModelLoader.cpp
Purpose: Making of implementing for ModelLoader System by using assimp and to easily load models
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

#include <assimp/postprocess.h>
#include <iostream>
#include <glm/glm.hpp>

#include "ModelLoader.h"
#include "ModelMesh.h"

ModelLoader* MODELLOAD = nullptr;

ModelLoader::ModelLoader()
{
	if (MODELLOAD == nullptr)
		MODELLOAD = this;
}

ModelLoader::~ModelLoader()
{
	models.clear();
}

void ModelLoader::Load(std::string name, std::string fileName)
{
	float minValue = std::numeric_limits<float>::max();
	float maxValue = std::numeric_limits<float>::min();
	data.min = glm::vec3(minValue, minValue, minValue);
	data.max = glm::vec3(maxValue, maxValue, maxValue);
	data.sumAllVertex = glm::vec3(0, 0, 0);

	vertex.clear();
	vertexNormal.clear();
	textureCoordinate.clear();
	indices.clear();

	std::string path = rootPath + fileName;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
	if (!Completed(scene))
		return;

	CreateNode(scene->mRootNode, scene);

	vertex = SetToUnitVertex(data, vertex);
	ModelMesh* mesh = new ModelMesh(vertex, vertexNormal, textureCoordinate, indices);
	mesh->Name = name;
	models.insert(std::make_pair(name, mesh));
}

ModelMesh* ModelLoader::Get(std::string name)
{
	if (IsExist(name))
		return models[name].get();

	return nullptr;
}

bool ModelLoader::Completed(const aiScene* scene)
{
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return false;
	}

	return true;
}

void ModelLoader::CreateNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		CreateMesh(mesh, scene);
	}

	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		CreateNode(node->mChildren[i], scene);
	}
}

void ModelLoader::CreateMesh(aiMesh* mesh, const aiScene* scene)
{
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;

		data.min = GetMin(data.min, vector);
		data.max = GetMax(data.max, vector);
		data.sumAllVertex += vector;

		vertex.push_back(vector);

		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertexNormal.push_back(vector);
		}

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 uv;
			uv.x = mesh->mTextureCoords[0][i].x;
			uv.y = mesh->mTextureCoords[0][i].y;
			textureCoordinate.push_back(uv);
		}
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
}

bool ModelLoader::IsExist(std::string name)
{
	return models.count(name) > 0;
}