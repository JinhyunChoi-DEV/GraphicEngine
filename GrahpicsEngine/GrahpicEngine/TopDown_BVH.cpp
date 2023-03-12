#include <algorithm>
#include "TopDown_BVH.h"
#include "BoundingVolumeHierarchy.h"

void TopDown_BVH::Create(std::vector<BoundingVolume> bvLists, BoundingVolumeType bvType_)
{
	bvType = bvType_;

	root_median_center.type = INTERNAL;
	root_median_center.parent = new BoundingVolume();
	root_median_center.parent->ActiveAABB = true;
	root_median_center.bvObjects = bvLists;
	root_median_center.left = nullptr;
	root_median_center.right = nullptr;
	root_median_center.parent->Expand(bvLists);
	ComputeTopDown(&root_median_center, 0, TopDownType::Median_Center);

	root_median_extents.type = INTERNAL;
	root_median_extents.parent = new BoundingVolume();
	root_median_extents.parent->ActiveAABB = true;
	root_median_extents.bvObjects = bvLists;
	root_median_extents.left = nullptr;
	root_median_extents.right = nullptr;
	root_median_extents.parent->Expand(bvLists);
	ComputeTopDown(&root_median_extents, 0, TopDownType::Median_Extents);

	root_k_even_splits.type = INTERNAL;
	root_k_even_splits.parent = new BoundingVolume();
	root_k_even_splits.parent->ActiveAABB = true;
	root_k_even_splits.bvObjects = bvLists;
	root_k_even_splits.left = nullptr;
	root_k_even_splits.right = nullptr;
	root_k_even_splits.parent->Expand(bvLists);
	ComputeTopDown(&root_k_even_splits, 0, TopDownType::K_Even_Splits);

}

void TopDown_BVH::Draw()
{
	if (ActiveMedianCenter)
		DrawNode(&root_median_center);

	if (ActiveMedianExtents)
		DrawNode(&root_median_extents);

	if (ActiveMedianKEvenSplit)
		DrawNode(&root_k_even_splits);
}

void TopDown_BVH::DrawNode(TreeNode* node)
{
	node->parent->Draw();

	if (node->left != nullptr)
		DrawNode(node->left);

	if (node->right != nullptr)
		DrawNode(node->right);
}

void TopDown_BVH::ComputeTopDown(TreeNode* node, int height, TopDownType type)
{
	node->height = height;
	node->parent->SetColor(BoundingVolumeHierarchy::hierarchyLevelColor[height]);

	if (BoundingVolumeHierarchy::MAX_HEIGHT == height || node->bvObjects.size() <= 1)
	{
		node->type = LEAF;
		return;
	}

	node->type = INTERNAL;
	node->left = new TreeNode();
	node->right = new TreeNode();

	SetLeftRightObjects(node, type);
	Partition(node);

	ComputeTopDown(node->left, height + 1, type);
	ComputeTopDown(node->right, height + 1, type);
}

void TopDown_BVH::SetLeftRightObjects(TreeNode* node, TopDownType type)
{
	float totalSize = (float)node->bvObjects.size();

	auto result_x = GetLeftRight(node, Axis::X, type);
	auto result_y = GetLeftRight(node, Axis::Y, type);
	auto result_z = GetLeftRight(node, Axis::Z, type);

	float left_P_x = abs(((float)std::get<0>(result_x).size() / totalSize) - 0.5f);
	float right_P_x = abs(((float)std::get<1>(result_x).size() / totalSize) - 0.5f);
	float left_P_y = abs(((float)std::get<0>(result_y).size() / totalSize) - 0.5f);
	float right_P_y = abs(((float)std::get<1>(result_y).size() / totalSize) - 0.5f);
	float left_P_z = abs(((float)std::get<0>(result_z).size() / totalSize) - 0.5f);
	float right_P_z = abs(((float)std::get<1>(result_z).size() / totalSize) - 0.5f);

	float percent_x = left_P_x + right_P_x;
	float percent_y = left_P_y + right_P_y;
	float percent_z = left_P_z + right_P_z;

	if (percent_x <= percent_y && percent_x <= percent_z)
	{
		node->left->bvObjects = std::get<0>(result_x);
		node->right->bvObjects = std::get<1>(result_x);
		return;
	}

	if (percent_y <= percent_x && percent_y <= percent_z)
	{
		node->left->bvObjects = std::get<0>(result_y);
		node->right->bvObjects = std::get<1>(result_y);
		return;
	}

	node->left->bvObjects = std::get<0>(result_z);
	node->right->bvObjects = std::get<1>(result_z);
}

std::tuple<BoundingVolumeList, BoundingVolumeList> TopDown_BVH::GetLeftRight(TreeNode* node, Axis axis, TopDownType type)
{
	BoundingVolumeList left;
	BoundingVolumeList right;

	int axisIndex = (int)axis;
	float splitPoint = 0.0f;
	if (type == TopDownType::Median_Center)
	{
		std::sort(node->bvObjects.begin(), node->bvObjects.end(),
			[&](BoundingVolume b1, BoundingVolume b2) { return b1.GetBV(bvType)->center[axisIndex] < b2.GetBV(bvType)->center[axisIndex]; });
		int mid = (int)(node->bvObjects.size() - 1) / 2;
		splitPoint = node->bvObjects[mid].GetBV(bvType)->center[axisIndex];

		for (auto bv : node->bvObjects)
		{
			auto currentBV = bv.GetBV(bvType);
			if (currentBV->center[axisIndex] <= splitPoint)
				left.push_back(bv);
			else
				right.push_back(bv);
		}
	}

	if (type == TopDownType::Median_Extents)
	{
		std::vector<float> extentsList;
		for (auto bv : node->bvObjects)
		{
			extentsList.push_back(bv.GetBV(bvType)->min[axisIndex]);
			extentsList.push_back(bv.GetBV(bvType)->max[axisIndex]);
		}
		std::sort(extentsList.begin(), extentsList.end());
		int mid = (int)(extentsList.size() - 1) / 2;
		splitPoint = extentsList[mid];

		for (auto bv : node->bvObjects)
		{
			auto currentBV = bv.GetBV(bvType);
			if (currentBV->center[axisIndex] <= splitPoint)
				left.push_back(bv);
			else
				right.push_back(bv);
		}

	}

	if (type == TopDownType::K_Even_Splits)
	{
		float min = FLT_MAX;
		float max = -FLT_MAX;
		for (auto bv : node->bvObjects)
		{
			auto currentBV = bv.GetBV(bvType);
			min = glm::min(min, currentBV->min[axisIndex]);
			max = glm::max(max, currentBV->max[axisIndex]);
		}

		std::sort(node->bvObjects.begin(), node->bvObjects.end(),
			[&](BoundingVolume b1, BoundingVolume b2) { return b1.GetBV(bvType)->center[axisIndex] < b2.GetBV(bvType)->center[axisIndex]; });

		int totalCount = (int)node->bvObjects.size();
		float step = (max - min) / (float)K;
		float bestRatio = FLT_MAX;
		for (int i = 0; i <= K; ++i)
		{
			splitPoint = (float)i * step + min;

			BoundingVolumeList tempLeft;
			BoundingVolumeList tempRight;
			for (auto bv : node->bvObjects)
			{
				auto currentBV = bv.GetBV(bvType);
				if (currentBV->center[axisIndex] <= splitPoint)
					tempLeft.push_back(bv);
				else
					tempRight.push_back(bv);
			}

			float leftPercent = abs(((float)tempLeft.size() / (float)totalCount) - 0.5f);
			float rightPercent = abs(((float)tempRight.size() / (float)totalCount) - 0.5f);
			float sumOfDiff = leftPercent + rightPercent;
			if (sumOfDiff < bestRatio)
			{
				bestRatio = sumOfDiff;
				left = tempLeft;
				right = tempRight;
			}

		}
	}

	std::sort(left.begin(), left.end(), [&](BoundingVolume b1, BoundingVolume b2) { return b1.GetBV(bvType)->center[axisIndex] < b2.GetBV(bvType)->center[axisIndex]; });
	std::sort(right.begin(), right.end(), [&](BoundingVolume b1, BoundingVolume b2) { return b1.GetBV(bvType)->center[axisIndex] < b2.GetBV(bvType)->center[axisIndex]; });
	return { left, right };
}

void TopDown_BVH::Partition(TreeNode* node)
{
	node->left->parent = new BoundingVolume;
	node->left->left = nullptr;
	node->left->right = nullptr;

	node->right->parent = new BoundingVolume;
	node->right->left = nullptr;
	node->right->right = nullptr;

	if (bvType == BoundingVolumeType::AABB)
		ExpendBox(node);
	else
		ExpendSphere(node);
}

void TopDown_BVH::ExpendBox(TreeNode* node)
{
	glm::vec3 leftMin(FLT_MAX);
	glm::vec3 leftMax(-FLT_MAX);
	for (auto bv : node->left->bvObjects)
	{
		leftMin = glm::min(leftMin, bv.GetBV(bvType)->min) + offset;
		leftMax = glm::max(leftMax, bv.GetBV(bvType)->max) - offset;
	}

	glm::vec3 rightMin(FLT_MAX);
	glm::vec3 rightMax(-FLT_MAX);
	for (auto bv : node->right->bvObjects)
	{
		rightMin = glm::min(rightMin, bv.GetBV(bvType)->min) + offset;
		rightMax = glm::max(rightMax, bv.GetBV(bvType)->max) - offset;
	}

	node->left->parent->Expand(leftMin, leftMax);
	node->right->parent->Expand(rightMin, rightMax);
}

void TopDown_BVH::ExpendSphere(TreeNode* node)
{
	for (auto bv : node->left->bvObjects)
	{
		node->left->parent->Expand(node->left->bvObjects);
	}

	for (auto bv : node->right->bvObjects)
	{
		node->right->parent->Expand(node->right->bvObjects);
	}
}