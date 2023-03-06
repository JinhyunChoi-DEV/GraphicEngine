#pragma once
#include <vector>
#include "BoundingVolume.h"

class BoundingVolume;
class TopDown_BVH
{
public:
	void Create(std::vector<BoundingVolume> bvLists);

	void Draw();

	BoundingVolume parent{};
};