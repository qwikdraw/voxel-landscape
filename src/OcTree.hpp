#pragma once

#include "voxel.hpp"

struct	OcTree
{
	int data;
	OcTree *branch[8];

	~OcTree(void);
	static OcTree	*Generate64(std::function<bool(glm::vec3)>, glm::vec3 center);
};
