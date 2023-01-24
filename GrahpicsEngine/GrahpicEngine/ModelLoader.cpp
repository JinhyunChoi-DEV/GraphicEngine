#include <iostream>
#include <glm/glm.hpp>

#include "ModelLoader.h"
#include "Mesh.h"

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
	vertex.clear();
	normal.clear();
	textureCoordinate.clear();
	indices.clear();

	std::string path = rootPath + fileName;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!Completed(scene))
		return;

	CreateNode(scene->mRootNode, scene);

	Mesh* mesh = new Mesh(vertex, normal, textureCoordinate, indices);
	models.insert(std::make_pair(name, mesh));
}

Mesh* ModelLoader::Get(std::string name)
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

		//std::cout << vector.x << ", " << vector.y << ", " << vector.z << std::endl;

		vertex.push_back(vector);

		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			normal.push_back(vector);
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
