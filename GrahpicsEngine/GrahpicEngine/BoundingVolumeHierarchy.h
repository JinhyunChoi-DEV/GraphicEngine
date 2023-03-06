#pragma once
#include <vector>

#include "BottomUp_BVH.h"
#include "TopDown_BVH.h"

enum NodeType
{
	INTERNAL, LEAF
};

class BoundingVolume;
struct Node
{
	NodeType type;
	BoundingVolume* data;
	int numObjects;
	Node* left, * right;
};

class BoundingVolumeHierarchy
{
public:
	BoundingVolumeHierarchy(std::vector<BoundingVolume> bvLists);

	void Draw();

private:
	TopDown_BVH topDown{};
	//BottomUp_BVH bottomUp;
};