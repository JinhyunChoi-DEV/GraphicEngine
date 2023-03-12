#pragma once
#include <vector>
#include "BoundingVolume.h"
#include "BoundingVolumeHierarchyHelper.h"

using BoundingVolumeList = std::vector<BoundingVolume>;
class BoundingVolume;
enum class TopDownType
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
	BoundingVolumeType bvType;
	float offset = 50;
	int K = 50;

	void DrawNode(TreeNode* node);

	void ComputeTopDown(TreeNode* node, int height, TopDownType type);
	void SetLeftRightObjects(TreeNode* node, TopDownType type);
	std::tuple<BoundingVolumeList, BoundingVolumeList> GetLeftRight(TreeNode* node, Axis axis, TopDownType type);

	void Partition(TreeNode* node);

	void ExpendBox(TreeNode* node);
	void ExpendSphere(TreeNode* node);
};
