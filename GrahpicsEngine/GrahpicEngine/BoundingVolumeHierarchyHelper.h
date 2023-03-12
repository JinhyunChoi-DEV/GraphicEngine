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