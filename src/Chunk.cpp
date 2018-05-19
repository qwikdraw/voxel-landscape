#include "Chunk.hpp"

Chunk::Chunk(glm::vec3 pos, OcTree *tree, size_t detail_level) : _tree(tree)
{

	_program = new ShadingProgram("src/chunkVertex.glsl", "", "src/chunkFrag.glsl");

	_program->Use();
	
	_lookAtID = glGetUniformLocation(_program->ID(), "lookAt");
	_projectionID = glGetUniformLocation(_program->ID(), "projection");
	_transformID = glGetUniformLocation(_program->ID(), "transform");

	_cubeVertices.insert(_cubeVertices.end(), {
					-0.5f,-0.5f,-0.5f,
					-0.5f,-0.5f, 0.5f,
					-0.5f, 0.5f, 0.5f,
						
					0.5f, 0.5f,-0.5f,
					-0.5f,-0.5f,-0.5f,
					-0.5f, 0.5f,-0.5f,
						
					0.5f,-0.5f, 0.5f,
					-0.5f,-0.5f,-0.5f,
					0.5f,-0.5f,-0.5f,
						
					0.5f, 0.5f,-0.5f,
					0.5f,-0.5f,-0.5f,
					-0.5f,-0.5f,-0.5f,
						
					-0.5f,-0.5f,-0.5f,
					-0.5f, 0.5f, 0.5f,
					-0.5f, 0.5f,-0.5f,
						
					0.5f,-0.5f, 0.5f,
					-0.5f,-0.5f, 0.5f,
					-0.5f,-0.5f,-0.5f,
						
					-0.5f, 0.5f, 0.5f,
					-0.5f,-0.5f, 0.5f,
					0.5f,-0.5f, 0.5f,
						
					0.5f, 0.5f, 0.5f,
					0.5f,-0.5f,-0.5f,
					0.5f, 0.5f,-0.5f,
						
					0.5f,-0.5f,-0.5f,
					0.5f, 0.5f, 0.5f,
					0.5f,-0.5f, 0.5f,
						
					0.5f, 0.5f, 0.5f,
					0.5f, 0.5f,-0.5f,
					-0.5f, 0.5f,-0.5f,
						
					0.5f, 0.5f, 0.5f,
					-0.5f, 0.5f,-0.5f,
					-0.5f, 0.5f, 0.5f,
						
					0.5f, 0.5f, 0.5f,
					-0.5f, 0.5f, 0.5f,
					0.5f,-0.5f, 0.5f
					});

	_cubeNormals.insert(_cubeNormals.end(), {
				-1, 0, 0,
				-1, 0, 0,
				-1, 0, 0,

				0, 0, -1,
				0, 0, -1,
				0, 0, -1,

				0, -1, 0,
				0, -1, 0,
				0, -1, 0,

				0, 0, -1,
				0, 0, -1,
				0, 0, -1,

				-1, 0, 0,
				-1, 0, 0,
				-1, 0, 0,

				0, -1, 0,
				0, -1, 0,
				0, -1, 0,

				0, 0, 1,
				0, 0, 1,
				0, 0, 1,

				1, 0, 0,
				1, 0, 0,
				1, 0, 0,

				1, 0, 0,
				1, 0, 0,
				1, 0, 0,

				0, 1, 0,
				0, 1, 0,
				0, 1, 0,

				0, 1, 0,
				0, 1, 0,
				0, 1, 0,

				0, 0, 1,
				0, 0, 1,
				0, 0, 1

		});
	
	getCubes(_tree, 0, detail_level, pos);

	glGenBuffers(1, &_cubeVertexID);
	glBindBuffer(GL_ARRAY_BUFFER, _cubeVertexID);
	glBufferData(GL_ARRAY_BUFFER,
		     _cubeVertices.size() * sizeof(GLfloat),
		     &_cubeVertices[0],
		     GL_STATIC_DRAW);

	glGenBuffers(1, &_cubeNormalID);
	glBindBuffer(GL_ARRAY_BUFFER, _cubeNormalID);
	glBufferData(GL_ARRAY_BUFFER,
		     _cubeNormals.size() * sizeof(GLfloat),
		     &_cubeNormals[0],
		     GL_STATIC_DRAW);
	
	glGenBuffers(1, &_positionID);
	glBindBuffer(GL_ARRAY_BUFFER, _positionID);
	glBufferData(GL_ARRAY_BUFFER,
		     _pos.size() * sizeof(GLfloat),
		     &_pos[0],
		     GL_STATIC_DRAW);

	glGenBuffers(1, &_sizeID);
	glBindBuffer(GL_ARRAY_BUFFER, _sizeID);
	glBufferData(GL_ARRAY_BUFFER,
		     _size.size() * sizeof(GLfloat),
		     &_size[0],
		     GL_STATIC_DRAW);

	std::cout << _totalCubes << std::endl;

	SetTransform(glm::mat4(1));
}

void	Chunk::getCubes(OcTree *tree, size_t depth_level, size_t detail_level, glm::vec3 center)
{
	static constexpr glm::vec3 offsets[8] = {{-0.5,  0.5, -0.5},
						 { 0.5,  0.5, -0.5},
						 {-0.5,  0.5,  0.5},
						 { 0.5,  0.5,  0.5},
						 {-0.5, -0.5, -0.5},
						 { 0.5, -0.5, -0.5},
						 {-0.5, -0.5,  0.5},
						 { 0.5, -0.5,  0.5}};
	
	if (!tree->branch[0] || depth_level == detail_level)
	{
		if (tree->data)
		{
			_pos.insert(_pos.end(), &center[0], &center[0] + 3);
			_size.push_back((unsigned)64 >> depth_level);
			_totalCubes++;
		}
		return;
	}
	for (size_t i = 0; i < 8; i++)
	{
		getCubes(tree->branch[i],
			 depth_level + 1,
			 detail_level,
			 center + offsets[i] * ((unsigned)64 >> (depth_level + 1)));
	}
}

void	Chunk::SetTransform(glm::mat4 m)
{
	glUniformMatrix4fv(_transformID, 1, GL_FALSE, glm::value_ptr(m));
}

void	Chunk::UsePerspective(std::pair<glm::mat4, glm::mat4> p)
{
	glUniformMatrix4fv(_lookAtID, 1, GL_FALSE, glm::value_ptr(p.first));
	glUniformMatrix4fv(_projectionID, 1, GL_FALSE, glm::value_ptr(p.second));
}

void	Chunk::Render(void)
{
	_program->Use();
	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _cubeVertexID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, _cubeNormalID);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, _positionID);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, _sizeID);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, 0);

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 0);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);

	glDrawArraysInstanced(GL_TRIANGLES, 0, 36, _totalCubes);

	glVertexAttribDivisor(2, 0);
	glVertexAttribDivisor(3, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);

}
