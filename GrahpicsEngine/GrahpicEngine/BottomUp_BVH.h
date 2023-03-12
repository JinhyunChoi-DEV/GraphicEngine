/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: BottomUp_BVH.h
Purpose: Making of header file for making Bottom Up method BVH
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

class BottomUp_BVH
{
public:
	void Create(std::vector<BoundingVolume> bvLists, BoundingVolumeType bvType_);

	void Draw();

	TreeNode* root;

private:
	void DrawNode(TreeNode* node);

	TreeNode* ComputeBottomUp(std::vector<TreeNode*>& nodes);
	void FindAndMerge(std::vector<TreeNode*> nodes, TreeNode*& left, TreeNode*& right);
	TreeNode* CreateParentNode(TreeNode* left, TreeNode* right);
	BoundingVolumeType bvType;
	float offset = 50;
};