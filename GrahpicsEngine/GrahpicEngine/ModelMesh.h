/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ModelMesh.h
Purpose: Making of header file for ModelMesh and this mesh aim for Object Models
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Mesh.h"

class Transform;

class ModelMesh : public Mesh
{
public:
	ModelMesh(std::vector<glm::vec3> vertex_, std::vector<glm::vec3> normal_,
		std::vector<glm::vec2> uv_, std::vector<unsigned int> indices_);
	ModelMesh(const ModelMesh& copy);
	~ModelMesh();

	void CopyData(const Mesh& copy) override;
	void Draw(Transform* transform) override;
	void DrawDebug(Transform* transform) override;

	bool DrawVertexNormal;
	bool DrawFaceNormal;
private:
	void DrawModel(Transform*);
	void DrawNormalLine(Transform*);

	void CreateModelBuffers();
	void CreateLineBuffers();
	void CreateFaceNormal();
	void CreateSphericalUV();
	void CreateNormalLines();
	void ClearBuffer();

	std::vector<glm::vec3> vertexNormal;
	std::vector<glm::vec3> faceNormal;
	std::vector<glm::vec2> textureCoordinate;
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> vertexNormalLine;
	std::vector<glm::vec3> faceNormalLine;

	unsigned int VAO = 0;
	unsigned int VBO[3] = { 0,0,0 };
	unsigned int EBO = 0;
	unsigned int lineVAO[2] = { 0,0 };
	unsigned int lineVBO[2] = { 0,0 };
};