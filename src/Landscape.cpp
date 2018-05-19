#include "Landscape.hpp"

Landscape::Landscape(std::function<bool(glm::vec3)> f) : _formula(f)
{
	_center = glm::vec3(0, 0, 0);

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			for (int k = 0; k < 5; k++)
			{
				glm::vec3 pos = {(i - 2) * 64, (j - 2) * 64, (k - 2) * 64};
				OcTree *tree = OcTree::Generate64(_formula, pos);
				
				_chunks[i][j][k] = new Chunk(pos, tree, 6);
			}
		}
	}
}

void	Landscape::Render(std::pair<glm::mat4, glm::mat4> perspective)
{
        for (int i = 0; i < 5; i++)
	{
         	for (int j = 0; j < 5; j++)
		{
                 	for (int k = 0; k < 5; k++)
			{
			 	_chunks[i][j][k]->UsePerspective(perspective);
				_chunks[i][j][k]->Render();
		        }
		}
	}
}
