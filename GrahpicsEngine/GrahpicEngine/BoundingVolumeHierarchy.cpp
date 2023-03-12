#include "BoundingVolumeHierarchy.h"
#include "BoundingVolume.h"
#include "TopDown_BVH.h"

int BoundingVolumeHierarchy::MAX_HEIGHT = 7;
BoundingVolumeHierarchy::BoundingVolumeHierarchy(std::vector<BoundingVolume> bvLists)
{
	hierarchyLevelColor[0] = glm::vec3(0.75, 0.38, 1);
	hierarchyLevelColor[1] = glm::vec3(0.62, 0.815, 1);
	hierarchyLevelColor[2] = glm::vec3(0.49, 1, 0.73);
	hierarchyLevelColor[3] = glm::vec3(1, 0.98, 0.68);
	hierarchyLevelColor[4] = glm::vec3(1, 0.54, 0.27);
	hierarchyLevelColor[5] = glm::vec3(1, 0.41, 0.45);
	hierarchyLevelColor[6] = glm::vec3(0.72, 1, 0.85);

	topDown_AABB.Create(bvLists, BoundingVolumeType::AABB);
	topDown_RitterBS.Create(bvLists, BoundingVolumeType::Ritter_BS);
	topDown_LarrsonBS.Create(bvLists, BoundingVolumeType::Larsson_BS);
	topDown_PCABS.Create(bvLists, BoundingVolumeType::PCA_BS);
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
}
