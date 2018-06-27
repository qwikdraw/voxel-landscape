#include "Landscape.hpp"

Landscape::Landscape(std::function<bool(glm::vec3)> f) : _formula(f)
{
	_center = glm::vec3(0, 0, 0);

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			for (int k = 0; k < 9; k++)
			{
				glm::vec3 pos = {(i - 4) * 64, (j - 4) * 64, (k - 4) * 64};
				OcTree *tree = OcTree::Generate64(_formula, pos);
				
				_chunks[i][j][k] = new Chunk(pos, tree, 6);
			}
		}
	}
}

static bool	within_view(glm::vec3& pos, glm::vec3& dir, glm::vec3& chunkpos)
{

	glm::vec3 v = chunkpos - pos;
	float dist = glm::length(v);

//	if (dist < 110)
//		return true;	
//	if (glm::dot(dir, v) > 0)
//		return false;
	if (dist > 300)
		return false;
	return true;
}

std::vector<Chunk*>	Landscape::ChunksToRender(glm::vec3 pos, glm::vec3 dir)
{
	std::vector<Chunk*> out;

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			for (int k = 0; k < 9; k++)
			{
				glm::vec3 chunkpos = _center +
					glm::vec3((i - 4) * 64, (j - 4) * 64, (k - 4) * 64);
				Chunk *c = _chunks[i][j][k];

				if (within_view(pos, dir, chunkpos))
					out.push_back(c);
			}
		}
	}

	//sort out based on distance from center... do later...

	return out;
}

void	Landscape::Render(std::pair<glm::mat4, glm::mat4> perspective)
{
	glm::vec3 pos = {glm::value_ptr(perspective.first)[12],
			 glm::value_ptr(perspective.first)[13],
			 glm::value_ptr(perspective.first)[14]};

	std::cout << "matrix" << std::endl;
	for (int i = 0; i < 16; i++)
	{
		if (i % 4 == 0)
			std::cout << std::endl;
		std::cout << glm::value_ptr(perspective.first)[i] << " ";
	}
	std::cout << std::endl << std::endl;
	
	glm::vec3 dir = {-glm::value_ptr(perspective.first)[8],
			 -glm::value_ptr(perspective.first)[9],
			 -glm::value_ptr(perspective.first)[10]};
	
	std::vector<Chunk*> renderList = ChunksToRender(pos, dir);

	for (Chunk *chunk : renderList)
	{
		chunk->UsePerspective(perspective);
		chunk->Render();
	}

}
