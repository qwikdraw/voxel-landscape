#pragma once

#include "Chunk.hpp"
#include "voxel.hpp"
#include <map>
#include <mutex>
#include <thread>
#include <queue>
#include <future>
#include <list>

class	ChunkLoader
{
	std::map<std::tuple<float, float, float>, Chunk*> _loadedChunks;
	std::function<bool(glm::vec3)> _formula;
	std::queue<glm::vec3> _waitingChunks;
	std::thread *_threads[4];
	std::mutex _mutex[2];
	std::promise<void> _promises[4];
	std::future<void> _futures[4];
	bool _threadClean[4];
	std::list<Chunk*> _chunksToFree;

	void	chunkCreator(int threadNum);

public:

	ChunkLoader(std::function<bool(glm::vec3)> formula);
	~ChunkLoader(void);

	// it is undefined to Add() more than one chunk at the same position
	// if no Clear() calls are used between them.
	void	Add(const glm::vec3& pos);
	Chunk	*Get(const glm::vec3& pos);
	void	Clear(void);

	// deletes some chunks, but not too many as to cause frame lag
	void	DeleteDeadChunks(void);
};
