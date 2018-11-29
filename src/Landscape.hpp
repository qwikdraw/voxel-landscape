#pragma once

#include "Chunk.hpp"
#include "util_inc.hpp"
#include "FreeCamera.hpp"
#include "ChunkLoader.hpp"

class	Landscape
{
	// size has to have odd number dimensions
	static constexpr int _sizeX = 15;
	static constexpr int _sizeY = 15;
	static const glm::ivec2 _size;

	// chunks that make up the landscape
	Chunk* _chunks[_sizeX][_sizeY] = {{nullptr}};

	// loads chunks on seperate threads
	ChunkLoader _chunkLoader;

	// the position of chunk _chunks[_sizeX / 2][_sizeY / 2]
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
	void	Render(const CameraUniforms& uniforms);
};
