#pragma once
#include <vector>

#include "TopDown_BVH.h"
#include "BottomUp_BVH.h"

class BoundingVolume;
class BoundingVolumeHierarchy
{
public:
	BoundingVolumeHierarchy(std::vector<BoundingVolume> bvLists);

	void Draw();

	static int MAX_HEIGHT;
	inline static glm::vec3 hierarchyLevelColor[7]{ glm::vec3(0) };

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