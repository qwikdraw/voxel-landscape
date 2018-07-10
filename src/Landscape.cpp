#include "Landscape.hpp"

constexpr glm::uvec3	Landscape::_size;

Landscape::Landscape(std::function<bool(glm::vec3)> f) : _formula(f),
							 _chunkLoader(f)
{
	_bufferIndex = 0;
	_center = glm::vec3(0, 0, 0);

	iterate(_size, [this](glm::uvec3 p)
	{
		glm::vec3 pos = {(float)p.x - _size.x / 2 + _center.x,
				 (float)p.y - _size.y / 2,
				 (float)p.z - _size.z / 2 + _center.z};
		pos *= 64;
		_chunks[_bufferIndex][p.x][p.y][p.z] = newChunk(pos);
	});
}

void	Landscape::iterate(const glm::uvec3& size, std::function<void(glm::uvec3)> f)
{
	for (size_t x = 0; x < size.x; x++)
		for (size_t y = 0; y < size.y; y++)
			for (size_t z = 0; z < size.z; z++)
				f(glm::uvec3(x, y, z));
}

Chunk	*Landscape::newChunk(const glm::vec3& pos)
{
	OcTree *tree = OcTree::Generate64(_formula, pos);
	Chunk *c = new Chunk(pos, tree, 6);
	c->Load();
	return c;
}

void	Landscape::updateCenter(glm::ivec3 center)
{
	_chunkLoader.Clear();
	
	int dx = _center.x - center.x;
	int dz = _center.z - center.z;
	
	iterate(_size, [&](glm::uvec3 p)
	{
		int xindex = dx + p.x;
		int zindex = dz + p.z;

		if (xindex >= 0 && xindex < _size.x && zindex >= 0 && zindex < _size.z)
			_chunks[!_bufferIndex][xindex][p.y][zindex] = _chunks[_bufferIndex][p.x][p.y][p.z];
		else
			delete _chunks[_bufferIndex][p.x][p.y][p.z];
		_chunks[_bufferIndex][p.x][p.y][p.z] = nullptr;
	});

	_center = center;
	_bufferIndex = !_bufferIndex;
	
	iterate(_size, [&](glm::uvec3 p)
	{
		if (!_chunks[_bufferIndex][p.x][p.y][p.z])
		{
			glm::vec3 pos = {(float)p.x - _size.x / 2 + _center.x,
					 (float)p.y - _size.y / 2,
					 (float)p.z - _size.z / 2 + _center.z};
			_chunkLoader.Add(pos);
		}
	});
	std::cout << "new center: " << _center.x << " " << _center.y << " " << _center.z << std::endl;
}

static bool	within_view(const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& chunkpos)
{

	glm::vec3 v = chunkpos - pos;
	float dist = glm::length(v);

//	if (dist < 110)
//		return true;	
//	if (glm::dot(dir, v) < 0)
//		return false;
	if (dist > 390)
		return false;
	return true;
}

std::vector<Chunk*>	Landscape::chunksToRender(const glm::vec3& pos, const glm::vec3& dir)
{
	std::vector<Chunk*> out;

	iterate(_size, [&](glm::uvec3 p)
	{
		Chunk *c = _chunks[_bufferIndex][p.x][p.y][p.z];
		if (!c)
			return;
		if (within_view(pos, dir, c->Pos()))
			out.push_back(c);
	});

	//sort out based on distance from camera location

	std::sort(out.begin(), out.end(), [pos](Chunk* a, Chunk* b)
	{
		glm::vec3 d1 = a->Pos() - pos;
		glm::vec3 d2 = b->Pos() - pos;

		return glm::dot(d1, d1) < glm::dot(d2, d2);
	});
	
	return out;
}

void	Landscape::Render(const Projection& projection)
{
	glm::ivec3 newCenter = glm::round(projection.position / 64);
	if (fabs(newCenter.x - _center.x) > 1 || fabs(newCenter.z - _center.z) > 1)
		updateCenter(newCenter);
	
	std::vector<Chunk*> renderList = chunksToRender(projection.position, projection.dir);

	for (Chunk *chunk : renderList)
	{
		chunk->Render(projection);
	}

	iterate(_size, [&](glm::uvec3 p)
	{
		if (!_chunks[_bufferIndex][p.x][p.y][p.z])
		{
			glm::vec3 pos = {(float)p.x - _size.x / 2 + _center.x,
					 (float)p.y - _size.y / 2,
					 (float)p.z - _size.z / 2 + _center.z};
			Chunk *c = _chunkLoader.Get(pos);

			if (c)
			{
				c->Load();
				_chunks[_bufferIndex][p.x][p.y][p.z] = c;
			}
		}
	});
}
