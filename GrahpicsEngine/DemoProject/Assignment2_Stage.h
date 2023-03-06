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
	void UpdateDebugViewGUI();

	bool activeAABB;
	bool activeRitterSphere;
	bool activeLarssonSphere;
	bool activePCAShpere;

	float lastUpdateTime = 0.0f;
	std::string sectionFileName = "Section4.txt";

	BoundingVolumeHierarchy* bvh = nullptr;
	std::vector<BoundingVolume*> bvLists;
};
