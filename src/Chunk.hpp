#pragma once

#include "OcTree.hpp"
#include "ShadingProgram.hpp"
#include "FreeCamera.hpp"
#include <mutex>

class	Chunk
{
	static std::mutex _mutex;
	static bool _init;
	static ShadingProgram *_program;

	OcTree *_tree;
	glm::vec3 _chunkPos;
	static std::vector<float> _cubeVertices;
	static std::vector<float> _cubeNormals;
	std::vector<float> _pos;
	std::vector<float> _size;
	int _totalCubes;

	static GLuint _cubeVertexID;
	static GLuint _cubeNormalID;
	GLuint _positionID;
	GLuint _sizeID;

	static GLuint _lookAtID;
	static GLuint _projectionID;
	static GLuint _transformID;

	void	getCubes(OcTree *tree, size_t depth_level, size_t detail_level, glm::vec3 center);
	void	useTransform(const glm::mat4& m);
	void	useProjection(const Projection& projection);

public:

	// the first time you create this object it must be inside the main thread
	Chunk(glm::vec3 pos, OcTree *data, size_t detail_level);

	~Chunk(void);

	// the stuff that should have been in the constructor but contained
	// OpenGL functions and so didn't allow creation of Chunk object in
	// seperate threads. Call Load() only in the main thread.
	void	Load(void);

	void	Render(const Projection& projection, const glm::mat4& transform = glm::mat4(1));
	const glm::vec3&	Pos(void);
};
