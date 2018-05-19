#pragma once

#include "OcTree.hpp"
#include "ShadingProgram.hpp"

class	Chunk
{
	ShadingProgram *_program;
	
	OcTree *_tree;
	std::vector<float> _cubeVertices;
	std::vector<float> _cubeNormals;
	std::vector<float> _pos;
	std::vector<float> _size;
	int _totalCubes;

	GLuint _cubeVertexID;
	GLuint _cubeNormalID;
	GLuint _positionID;
	GLuint _sizeID;
	
	GLuint _lookAtID;
	GLuint _projectionID;
	GLuint _transformID;

	void	getCubes(OcTree *tree, size_t depth_level, size_t detail_level, glm::vec3 center);
	
public:

	Chunk(glm::vec3 pos, OcTree *data, size_t detail_level);

	void	SetTransform(glm::mat4);
	void	UsePerspective(std::pair<glm::mat4, glm::mat4>);
	void	Render(void);
};
