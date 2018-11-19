#pragma once

#include "Chunk.hpp"
#include "voxel.hpp"
#include "FreeCamera.hpp"
#include "ChunkLoader.hpp"

class	Landscape
{
	constexpr static glm::uvec3 _size = {9, 4, 9};
	
	Chunk *_chunks[2][_size.x][_size.y][_size.z] = {nullptr};
	bool _bufferIndex;
	ChunkLoader _chunkLoader;
	
	glm::ivec3 _center;
	std::function<bool(glm::vec3)> _formula;

	static void	iterate(const glm::uvec3& size, std::function<void(glm::uvec3)>);

	Chunk	*newChunk(const glm::vec3& pos);
	void	updateCenter(glm::ivec3 center);	
	std::vector<Chunk*>	chunksToRender(const glm::vec3& pos, const glm::vec3& dir);
	
public:

	Landscape(std::function<bool(glm::vec3)>);
	void	Render(const Projection& projection);
};
