/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: TopDown_BVH.h
Purpose: Making of header file for making Top Down method BVH
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 3/12/2023
End Header --------------------------------------------------------*/

#pragma once
#include <vector>
#include "BoundingVolume.h"
#include "BoundingVolumeHierarchyHelper.h"

using BoundingVolumeList = std::vector<BoundingVolume>;
class BoundingVolume;
enum class TopDownSplitType
{
	Median_Center, Median_Extents, K_Even_Splits
};

class TopDown_BVH
{
public:
	void Create(std::vector<BoundingVolume> bvLists, BoundingVolumeType bvType_);

	void Draw();

	TreeNode root_median_center;
	TreeNode root_median_extents;
	TreeNode root_k_even_splits;

	bool ActiveMedianCenter;
	bool ActiveMedianExtents;
	bool ActiveMedianKEvenSplit;

private:
	void DrawNode(TreeNode* node);
	void ComputeTopDown(TreeNode* node, int height, TopDownSplitType type);
	void SetLeftRightObjects(TreeNode* node, TopDownSplitType type);
	std::tuple<BoundingVolumeList, BoundingVolumeList> GetLeftRight(TreeNode* node, Axis axis, TopDownSplitType type);
	void Partition(TreeNode* node);
	void ExpendBox(TreeNode* node);
	void ExpendSphere(TreeNode* node);

	int MAX_HEIGHT = 0;
	glm::vec3 hierarchyLevelColor[7]{ glm::vec3(0) };
	BoundingVolumeType bvType;
	float offset = 50;
	int K = 50;
};
