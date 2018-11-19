#include "ChunkLoader.hpp"

ChunkLoader::ChunkLoader(std::function<bool(glm::vec3)> formula) : _formula(formula)
{
	for (int i = 0; i < 4; i++)
	{
		_futures[i] = _promises[i].get_future();
		_threads[i] = new std::thread(&ChunkLoader::chunkCreator, this, i);
		_threadClean[i] = false;
	}
}

ChunkLoader::~ChunkLoader(void)
{
	Clear();
	for (int i = 0; i < 4; i++)
		_promises[i].set_value();
	for (int i = 0; i < 4; i++)
		_threads[i]->join();
}

static std::tuple<float, float, float>	map_key(const glm::vec3& pos)
{
	return std::make_tuple(pos.x, pos.y, pos.z);
}

void	ChunkLoader::chunkCreator(int threadNum)
{
	while (_futures[threadNum].wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
	{
		_mutex[0].lock();
		if (_threadClean[threadNum])
			_threadClean[threadNum] = false;
		if (_waitingChunks.empty())
		{
			_mutex[0].unlock();
			continue;
		}
		glm::vec3 pos = _waitingChunks.front();
		_waitingChunks.pop();
		_mutex[0].unlock();

		OcTree *tree = OcTree::Generate64(_formula, pos);
		Chunk *c = new Chunk(pos, tree, 6);

		_mutex[1].lock();
		if (_threadClean[threadNum])
			_chunksToFree.push_back(c);
		else
			_loadedChunks[map_key(pos)] = c;
		_mutex[1].unlock();
	}
}

void	ChunkLoader::Add(const glm::vec3& pos)
{
	_mutex[0].lock();
	_waitingChunks.push(pos);
	_mutex[0].unlock();
}

Chunk	*ChunkLoader::Get(const glm::vec3& pos)
{
	_mutex[1].lock();
	if (_loadedChunks.count(map_key(pos)) == 0)
	{
		_mutex[1].unlock();
		return nullptr;
	}
	Chunk *c =  _loadedChunks[map_key(pos)];
	_loadedChunks.erase(map_key(pos));
	_mutex[1].unlock();
	return c;
}

void	ChunkLoader::Clear(void)
{
	_mutex[1].lock();
	for (auto const& p : _loadedChunks)
		_chunksToFree.push_back(p.second);
	_loadedChunks.clear();
	_waitingChunks = std::queue<glm::vec3>();
	for (int i = 0; i < 4; i++)
		_threadClean[i] = true;
	_mutex[1].unlock();
}

void	ChunkLoader::DeleteDeadChunks(void)
{
	if (_chunksToFree.empty())
		return;
	delete _chunksToFree.front();

	_mutex[1].lock();
	_chunksToFree.pop_front();
	_mutex[1].unlock();
}
