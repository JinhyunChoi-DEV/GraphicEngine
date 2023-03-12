/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: BottomUp_BVH.cpp
Purpose: Making of source file for making Bottom Up method BVH
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 3/12/2023
End Header --------------------------------------------------------*/

#include <algorithm>
#include "Random.h"
#include "BottomUp_BVH.h"

void BottomUp_BVH::Create(std::vector<BoundingVolume> bvLists, BoundingVolumeType bvType_)
{
	bvType = bvType_;

	std::vector<TreeNode*> nodes;

	glm::vec3 color;
	if (BoundingVolume::BottomUpColors.empty())
	{
		color = GetRandomColor();
		BoundingVolume::BottomUpColors.push_back(color);
	}
	else
	{
		color = BoundingVolume::BottomUpColors[0];
	}

	for (auto bv : bvLists)
	{
		TreeNode* node = new TreeNode();
		node->currentBV = new BoundingVolume();
		node->currentBV->GetBV(bvType_)->Expand(bv);
		node->currentBV->ActiveByType(bvType_);
		node->currentBV->SetColor(color);
		node->bvObjects.push_back(bv);
		node->left = nullptr;
		node->right = nullptr;
		node->type = LEAF;
		node->level = 0;
		nodes.push_back(node);
	}

	root = ComputeBottomUp(nodes);
}

void BottomUp_BVH::Draw()
{
	DrawNode(root);
}

void BottomUp_BVH::DrawNode(TreeNode* node)
{
	node->currentBV->Draw();

	if (node->left != nullptr)
		DrawNode(node->left);

	if (node->right != nullptr)
		DrawNode(node->right);
}

TreeNode* BottomUp_BVH::ComputeBottomUp(std::vector<TreeNode*>& nodes)
{
	while (nodes.size() > 1)
	{
		TreeNode* left = nullptr, * right = nullptr;

		FindAndMerge(nodes, left, right);

		TreeNode* parent = CreateParentNode(left, right);

		// remove left and right node from nodes vector
		nodes.erase(std::remove_if(nodes.begin(), nodes.end(), [&](const TreeNode* n) { return n == left; }), nodes.end());
		nodes.erase(std::remove_if(nodes.begin(), nodes.end(), [&](const TreeNode* n) { return n == right; }), nodes.end());

		// add parent node to the vector
		nodes.push_back(parent);
	}

	return nodes[0];
}

void BottomUp_BVH::FindAndMerge(std::vector<TreeNode*> nodes, TreeNode*& left, TreeNode*& right)
{
	int totalSize = (int)nodes.size();
	float bestSoFar = FLT_MAX;
	for (int i = 0; i < totalSize; ++i)
	{
		auto target = nodes[i]->currentBV;
		for (int j = i + 1; j < totalSize; ++j)
		{
			auto current = nodes[j]->currentBV;
			float distance = glm::distance(target->GetBV(bvType)->center, current->GetBV(bvType)->center);
			if (fabs(bestSoFar - distance) < 0.0001f)
			{
				BoundingVolume bv1;
				BoundingVolume bv2;
				bv1.Expand({ *left->currentBV, *right->currentBV });
				bv2.Expand({ *nodes[i]->currentBV, *nodes[j]->currentBV });

				float v1 = bv1.GetBV(bvType)->Volume();
				float v2 = bv2.GetBV(bvType)->Volume();

				if (v2 < v1)
				{
					left = nodes[i];
					right = nodes[j];
				}
			}
			else if (distance < bestSoFar)
			{
				bestSoFar = distance;
				left = nodes[i];
				right = nodes[j];
			}
		}
	}
}

TreeNode* BottomUp_BVH::CreateParentNode(TreeNode* left, TreeNode* right)
{
	glm::vec3 color = GetRandomColor();

	TreeNode* result = new TreeNode();
	result->left = left;
	result->right = right;
	result->bvObjects.insert(result->bvObjects.end(), left->bvObjects.begin(), left->bvObjects.end());
	result->bvObjects.insert(result->bvObjects.end(), right->bvObjects.begin(), right->bvObjects.end());

	// set offset preventing overlap lines
	left->currentBV->GetBV(bvType)->min -= offset;
	left->currentBV->GetBV(bvType)->max += offset;
	right->currentBV->GetBV(bvType)->min -= offset;
	right->currentBV->GetBV(bvType)->max += offset;
	// end the offset

	result->currentBV = new BoundingVolume();
	result->currentBV->ActiveByType(bvType);
	result->currentBV->SetColor(color);
	result->currentBV->Expand({ *left->currentBV, *right->currentBV });
	result->type = INTERNAL;
	result->level = std::min(left->level, right->level) + 1;


	if (BoundingVolume::BottomUpColors.size() > result->level)
	{
		result->currentBV->SetColor(BoundingVolume::BottomUpColors[result->level]);
	}
	else
	{
		BoundingVolume::BottomUpColors.push_back(color);
		result->currentBV->SetColor(color);
	}

	return result;
}
