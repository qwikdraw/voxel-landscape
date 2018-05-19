#pragma once

#include "Chunk.hpp"
#include "voxel.hpp"

class	Landscape
{

	Chunk *_chunks[1][1][1]; // _chunks[4][4][4] is the center
	glm::vec3 _center;
	std::function<bool(glm::vec3)> _formula;

	
public:

	Landscape(std::function<bool(glm::vec3)>);
	void	Render(std::pair<glm::mat4, glm::mat4> perspective);
};
