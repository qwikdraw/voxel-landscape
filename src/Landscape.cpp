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

static bool	within_view(const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& chunkpos)
{

	glm::vec3 v = chunkpos - pos;
	float dist = glm::length(v);

	if (dist < 110)
		return true;	
	if (glm::dot(dir, v) > 0)
		return false;
	if (dist > 300)
		return false;
	return true;
}

std::vector<Chunk*>	Landscape::ChunksToRender(const glm::vec3& pos, const glm::vec3& dir)
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

void	Landscape::Render(const Projection& projection)
{
	std::vector<Chunk*> renderList = ChunksToRender(projection.position, projection.dir);

	for (Chunk *chunk : renderList)
	{
		chunk->Render(projection);
	}

}
