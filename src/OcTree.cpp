#include "OcTree.hpp"

OcTree::~OcTree(void)
{
	for (auto b : branch)
		delete b;
}

static void	fill(OcTree *tree,
		     size_t depth_level,
		     glm::vec3 center,
		     std::function<int(glm::vec3)> f)
{
	static constexpr glm::vec3 offsets[8] = {{-0.5,  0.5, -0.5},
						 { 0.5,  0.5, -0.5},
						 {-0.5,  0.5,  0.5},
						 { 0.5,  0.5,  0.5},
						 {-0.5, -0.5, -0.5},
						 { 0.5, -0.5, -0.5},
						 {-0.5, -0.5,  0.5},
						 { 0.5, -0.5,  0.5}};

	if (depth_level == 0)
	{
		for (int i = 0; i < 8; i++)
			tree->branch[i] = nullptr;
		tree->data = (int)f(center) * 8;
		return;
	}

	tree->data = 0;
	
	for (int i = 0; i < 8; i++)
	{
		tree->branch[i] = new OcTree;
		fill(tree->branch[i],
		     depth_level - 1,
		     center + offsets[i] * (1 << (depth_level - 1)),
		     f);
		tree->data += tree->branch[i]->data;
	}
	if (tree->data != 0)
	{
		tree->data /= 8;
		if (tree->data == 0)
			tree->data = 1;
	}
}

static void	compress(OcTree *tree)
{
	if (tree->data == 8 || tree->data == 0)
	{
		for (int i = 0; i < 8; i++)
		{
			delete tree->branch[i];
			tree->branch[i] = nullptr;
		}
		return;
	}
	
	assert(tree->branch[0]);
	
	for (int i = 0; i < 8; i++)
		compress(tree->branch[i]);
}

OcTree	*OcTree::Generate64(std::function<bool(glm::vec3)> f, glm::vec3 center)
{
	OcTree *ot = new OcTree;

	fill(ot, 6, center, f);
	compress(ot);
	return ot;
}
