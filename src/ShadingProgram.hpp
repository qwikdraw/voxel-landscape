#pragma once

#include "util_inc.hpp"

class	ShadingProgram
{
private:

	GLuint _program;

	std::string	GetShaderCode(std::string filepath);
	void	CheckCompilation(GLuint, std::string filepath);
	void	CheckLinking(void);

public:
	ShadingProgram(std::string vertexPath, std::string fragPath);
	void	Use(void);
	GLuint	ID(void);
};
