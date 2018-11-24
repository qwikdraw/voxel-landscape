#pragma once

#include "util_inc.hpp"
#include "FreeCamera.hpp"
#include "Texture.hpp"
#include "ShadingProgram.hpp"

class	SkyBox
{
	static constexpr const char* _vertexPath = "src/skybox_vert.glsl";
    static constexpr const char* _fragPath = "src/skybox_frag.glsl";

	static const std::vector<float> _vertexArray;

	ShadingProgram *_program;
	GLuint _vertexArrayID;
	GLuint _textureID;
	GLuint _textureLocationID;
	GLuint _projectionID;
	GLuint _VAO;

	void _loadArrayBuffers();
	void _loadTextures(const std::vector<Texture*>&);
	void _makeVAO();

public:
	SkyBox(std::string right, std::string left, std::string top,
	       std::string bot, std::string back, std::string front);
	~SkyBox(void);
	void	Render(const Projection& projection);
};
