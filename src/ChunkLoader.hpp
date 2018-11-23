#pragma once

#include "Chunk.hpp"
#include "util_inc.hpp"
#include <map>
#include <mutex>
#include <thread>
#include <queue>
#include <future>
#include <list>


// Load chunks by calling Add(), then Get(). If you want to cancel any
// orders, call Clear() to cancel. To delete the canceled chunks
// DeleteDeadChunks() will try to delete one of the dead chunks.
// Once you succesfully call Get() on a chunk you own it and Clear() +
// DeleteDeadChunks() will not effect it.
class	ChunkLoader
{
	// tuple because glm doesn't support < operator on vector types
	std::map<std::tuple<int, int>, Chunk*> _loadedChunks;

	// positions of chunks that need to be created
	std::queue<glm::ivec2> _waitingChunks;

	// thread stuff
	std::thread *_threads[4];
	std::mutex _mutex[2];
	std::promise<void> _promises[4];
	std::future<void> _futures[4];

	// indicates if a thread should stop loading a chunk and get rid
	// of it. Used because of small edge case with Clear and memory leaks
	bool _threadClean[4];

	// list of chunks that will be deleted by DeleteDeadChunks eventually
	std::list<Chunk*> _chunksToFree;

	// the worker thread loop
	void	_chunkCreator(size_t threadNum);

public:

	// spawns 4 worker threads that will constantly check for and create
	// chunks
	ChunkLoader();

	// ends and joins the worker threads
	~ChunkLoader(void);

	// it is undefined to Add() more than one chunk at the same position
	// if no Clear() calls are used between them... just don't do it...
	void	Add(const glm::ivec2& pos);

	// attempt to get a chunk. If it hasn't been loaded will return
	// nullptr
	Chunk	*Get(const glm::ivec2& pos);

	// tell chunkloader to clean all chunks that havent been got yet.
	void	Clear(void);

	// deletes a single chunk and calls unload on it. Only deletes 1
	// because this action is done on main thread and so aims to avoid
	// frame lag
	void	DeleteDeadChunks(void);
};
