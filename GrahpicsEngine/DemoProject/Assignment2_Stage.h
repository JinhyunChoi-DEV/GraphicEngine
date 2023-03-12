/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Assignment2_Stage.h
Purpose: Making of header file for making Assignment2 scene
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 3/12/2023
End Header --------------------------------------------------------*/

#pragma once
#include <string>

#include "BoundingVolumeHierarchy.h"

class BoundingVolume;
class Assignment2_Stage
{
public:
	Assignment2_Stage();

	void Initialize();
	void Update();
	void Close();

private:
	void LoadPlantSection();
	void SetBaseLight();
	void CreateCamera();
	void UpdateCamera(float dt);
	void CreateBVH();

	void InitializeGUI();
	void UpdateGUI();
	void UpdateBV_Active();
	void UpdateBVH_Active();
	void UpdateDebugViewGUI();

	bool activeAABB;
	bool activeRitterSphere;
	bool activeLarssonSphere;
	bool activePCAShpere;

	float lastUpdateTime = 0.0f;
	std::string sectionFileName = "Section4.txt";
	std::vector<std::string> splitPointMethodList;
	std::string currentSplitPointMethod;

	BoundingVolumeHierarchy* bvh = nullptr;
	std::vector<BoundingVolume*> bvLists;
};
