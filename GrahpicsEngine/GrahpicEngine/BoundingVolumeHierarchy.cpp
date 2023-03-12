#include "BoundingVolumeHierarchy.h"
#include "BoundingVolume.h"
#include "TopDown_BVH.h"

BoundingVolumeHierarchy::BoundingVolumeHierarchy(std::vector<BoundingVolume> bvLists)
{
	topDown_AABB.Create(bvLists, BoundingVolumeType::AABB);
	topDown_RitterBS.Create(bvLists, BoundingVolumeType::Ritter_BS);
	topDown_LarrsonBS.Create(bvLists, BoundingVolumeType::Larsson_BS);
	topDown_PCABS.Create(bvLists, BoundingVolumeType::PCA_BS);

	bottomUp_AABB.Create(bvLists, BoundingVolumeType::AABB);
	bottomUp_RitterBS.Create(bvLists, BoundingVolumeType::Ritter_BS);
	bottomUp_LarrsonBS.Create(bvLists, BoundingVolumeType::Larsson_BS);
	bottomUp_PCABS.Create(bvLists, BoundingVolumeType::PCA_BS);
}

void BoundingVolumeHierarchy::SetTopdownSplitMethod(TopDownSplitType type)
{
	topDown_AABB.ActiveMedianCenter = type == TopDownSplitType::Median_Center;
	topDown_AABB.ActiveMedianExtents = type == TopDownSplitType::Median_Extents;
	topDown_AABB.ActiveMedianKEvenSplit = type == TopDownSplitType::K_Even_Splits;

	topDown_RitterBS.ActiveMedianCenter = type == TopDownSplitType::Median_Center;
	topDown_RitterBS.ActiveMedianExtents = type == TopDownSplitType::Median_Extents;
	topDown_RitterBS.ActiveMedianKEvenSplit = type == TopDownSplitType::K_Even_Splits;

	topDown_LarrsonBS.ActiveMedianCenter = type == TopDownSplitType::Median_Center;
	topDown_LarrsonBS.ActiveMedianExtents = type == TopDownSplitType::Median_Extents;
	topDown_LarrsonBS.ActiveMedianKEvenSplit = type == TopDownSplitType::K_Even_Splits;

	topDown_PCABS.ActiveMedianCenter = type == TopDownSplitType::Median_Center;
	topDown_PCABS.ActiveMedianExtents = type == TopDownSplitType::Median_Extents;
	topDown_PCABS.ActiveMedianKEvenSplit = type == TopDownSplitType::K_Even_Splits;
}

void BoundingVolumeHierarchy::Draw()
{
	if (ActiveTopdown_AABB)
		topDown_AABB.Draw();

	if (ActiveTopdown_RitterBS)
		topDown_RitterBS.Draw();

	if (ActiveTopdown_LarrsonBS)
		topDown_LarrsonBS.Draw();

	if (ActiveTopdown_PCABS)
		topDown_PCABS.Draw();

	if (ActiveBottomUp_AABB)
		bottomUp_AABB.Draw();

	if (ActiveBottomUp_RitterBS)
		bottomUp_RitterBS.Draw();

	if (ActiveBottomUp_LarrsonBS)
		bottomUp_LarrsonBS.Draw();

	if (ActiveBottomUp_PCABS)
		bottomUp_PCABS.Draw();
}
