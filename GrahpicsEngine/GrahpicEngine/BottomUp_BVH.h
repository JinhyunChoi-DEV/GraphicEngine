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