#include "Landscape.hpp"

const glm::ivec2 Landscape::_size = {_sizeX, _sizeY};

Landscape::Landscape()
{
	_center = glm::ivec2(0);
	for (int x = -_sizeX / 2; x <= _sizeX / 2; x++)
		for (int y = -_sizeY / 2; y <= _sizeY / 2; y++)
			_chunkLoader.Add(glm::ivec2(x * 32, y * 32));
}

std::vector<Chunk*>	Landscape::_chunksToRender(const glm::vec2& pos, const glm::vec2& dir)
{
	static constexpr float maxChunkDist = 200.0;
	std::vector<Chunk*> out;

	// add every chunk that is within maxChunkDist
	// distance from camera
	for (size_t x = 0; x < _sizeX; x++)
	{
		for (size_t y = 0; y < _sizeY; y++)
		{
			if (_chunks[x][y])
			{
				auto chunkpos = glm::vec2(_chunks[x][y]->Pos());
				// auto chunkcull = pos - dir * 32 * glm::sqrt(2);
				if (/* glm::dot(chunkpos - chunkcull, dir) > 0 && */
					glm::length(chunkpos - pos) < maxChunkDist)
				{
					out.push_back(_chunks[x][y]);
				}
			}
		}
	}

	// sort the chunks from closest to furthest from camera. Allows
	// early culling optimization from OpenGL
	auto comp = [pos](Chunk* a, Chunk* b)
	{
		glm::vec2 d1 = glm::vec2(a->Pos()) - pos;
		glm::vec2 d2 = glm::vec2(b->Pos()) - pos;
		return glm::length2(d1) < glm::length2(d2);
	};
	std::sort(out.begin(), out.end(), comp);

	return out;
}

void Landscape::_updateCenter(glm::ivec2 newCenter)
{
	glm::ivec2 diff = newCenter - _center;

	_chunkLoader.Clear();
	Chunk* tempChunks[_sizeX][_sizeY] = {{nullptr}};
	for (int x = 0; x < _sizeX; x++)
	{
		for (int y = 0; y < _sizeY; y++)
		{
			if (x - diff.x >= 0 && x - diff.x < _sizeX &&
				y - diff.y >= 0 && y - diff.y < _sizeY)
			{
				tempChunks[size_t(x - diff.x)][size_t(y - diff.y)] =
					_chunks[size_t(x)][size_t(y)];
			}
			else if (_chunks[size_t(x)][size_t(y)])
			{
					_chunks[size_t(x)][size_t(y)]->Unload();
					delete _chunks[size_t(x)][size_t(y)];
			}
		}
	}
	std::memmove(&_chunks, &tempChunks, sizeof(_chunks));
	_center = newCenter;
	for (int x = 0; x < _sizeX; x++)
		for (int y = 0; y < _sizeY; y++)
			if (!_chunks[x][y])
				_chunkLoader.Add(glm::ivec2(x - _sizeX / 2, y - _sizeY / 2) * 32 + _center * 32);
}

void	Landscape::Render(const CameraUniforms& uniforms)
{
	glm::ivec2 newCenter = glm::round(glm::vec2(uniforms.position.x, uniforms.position.z) / 32);
	if (abs(newCenter.x - _center.x) >= 1 || abs(newCenter.y - _center.y) >= 1)
		_updateCenter(newCenter);

	std::vector<Chunk*> renderList = _chunksToRender(
		glm::vec2(uniforms.position.x, uniforms.position.z),
		glm::vec2(uniforms.direction.x, uniforms.direction.z));

	Chunk::Render(uniforms, renderList);

	for (size_t x = 0; x < _sizeX; x++)
	{
		for (size_t y = 0; y < _sizeY; y++)
		{
			if (!_chunks[x][y])
			{
				glm::ivec2 pos = (glm::ivec2(x, y) - _size / 2 + _center) * 32;
				Chunk *c = _chunkLoader.Get(pos);
				if (c)
				{
					c->Load();
					_chunks[x][y] = c;
				}
			}
		}
	}
	_chunkLoader.DeleteDeadChunks();
}
