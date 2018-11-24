#pragma once

#include "Chunk.hpp"
#include "util_inc.hpp"
#include "FreeCamera.hpp"
#include "ChunkLoader.hpp"

class	Landscape
{
	// size has to have odd number dimensions
	constexpr const static glm::ivec2 _size = {15, 15};

	// chunks that make up the landscape
	Chunk* _chunks[_size.x][_size.y] = {nullptr};

	// loads chunks on seperate threads
	ChunkLoader _chunkLoader;

	// the position of chunk _chunks[_size.x / 2][_size.y / 2]
	glm::ivec2 _center;

	// uses camera position and direction to choose only the chunks needed to
	// be rendered
	std::vector<Chunk*>	_chunksToRender(const glm::vec2& pos, const glm::vec2& dir);

	// updates the center of the map, requesting new chunks to be loaded
	// and deleting old chunks
	void _updateCenter(glm::ivec2);

public:

	// initializes landscape with a center of 0, 0 and requests for new chunks
	// to be loaded
	Landscape();

	// gets any loaded chunks, renders any chunks _chunksToRender, updates
	// center
	void	Render(const Projection& projection);
};
