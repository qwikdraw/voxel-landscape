#pragma once

#include "Chunk.hpp"
#include "voxel.hpp"
#include "FreeCamera.hpp"

class	Landscape
{

	Chunk *_chunks[9][9][9]; // _chunks[4][4][4] is the center
	glm::vec3 _center;
	std::function<bool(glm::vec3)> _formula;

	std::vector<Chunk*>	ChunksToRender(const glm::vec3& pos, const glm::vec3& dir);
	
public:

	Landscape(std::function<bool(glm::vec3)>);
	void	Render(const Projection& projection);
};
