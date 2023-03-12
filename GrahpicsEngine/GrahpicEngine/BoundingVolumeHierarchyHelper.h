/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: BoundingVolumeHierarchyHelper.h
Purpose: Making of header file for making helper data about bounding volume hierarchy
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 3/12/2023
End Header --------------------------------------------------------*/

#pragma once

enum NodeType
{
	INTERNAL, LEAF
};

enum class Axis
{
	X = 0, Y, Z
};

enum class BoundingVolumeType
{
	AABB, Ritter_BS, Larsson_BS, PCA_BS
};

struct TreeNode
{
	NodeType type;
	BoundingVolume* currentBV;
	std::vector<BoundingVolume> bvObjects;
	TreeNode* left, * right;
	int level = 0;
};