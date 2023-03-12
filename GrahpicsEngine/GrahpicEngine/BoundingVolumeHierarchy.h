/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: BoundingVolumeHierarchy.h
Purpose: Making of header file for implementing master bounding volume hierarchy controller
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 3/12/2023
End Header --------------------------------------------------------*/

#pragma once
#include <vector>

#include "TopDown_BVH.h"
#include "BottomUp_BVH.h"

class BoundingVolume;
class BoundingVolumeHierarchy
{
public:
	BoundingVolumeHierarchy(std::vector<BoundingVolume> bvLists);

	void SetTopdownSplitMethod(TopDownSplitType type);
	void Draw();

	bool ActiveTopdown_AABB;
	bool ActiveTopdown_RitterBS;
	bool ActiveTopdown_LarrsonBS;
	bool ActiveTopdown_PCABS;

	bool ActiveBottomUp_AABB;
	bool ActiveBottomUp_RitterBS;
	bool ActiveBottomUp_LarrsonBS;
	bool ActiveBottomUp_PCABS;

private:
	TopDown_BVH topDown_AABB;
	TopDown_BVH topDown_RitterBS;
	TopDown_BVH topDown_LarrsonBS;
	TopDown_BVH topDown_PCABS;

	BottomUp_BVH bottomUp_AABB;
	BottomUp_BVH bottomUp_RitterBS;
	BottomUp_BVH bottomUp_LarrsonBS;
	BottomUp_BVH bottomUp_PCABS;
};