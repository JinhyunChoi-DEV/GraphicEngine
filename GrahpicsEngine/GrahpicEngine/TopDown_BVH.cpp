#include "TopDown_BVH.h"

void TopDown_BVH::Create(std::vector<BoundingVolume> bvLists)
{
	parent.ActiveAABB = true;
	for (auto bv : bvLists)
	{
		parent.Expand(bv);
	}

}

void TopDown_BVH::Draw()
{
	parent.Draw();
}
