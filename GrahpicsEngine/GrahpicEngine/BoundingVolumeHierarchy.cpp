#include "BoundingVolumeHierarchy.h"
#include "BoundingVolume.h"

BoundingVolumeHierarchy::BoundingVolumeHierarchy(std::vector<BoundingVolume> bvLists)
{
	topDown.Create(bvLists);
	int a = 5;
}

void BoundingVolumeHierarchy::Draw()
{
	//topDown.Draw();
}
