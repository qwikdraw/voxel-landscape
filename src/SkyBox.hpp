#pragma once

#include "util_inc.hpp"
#include "FreeCamera.hpp"
#include "Texture.hpp"
#include "ShadingProgram.hpp"

class	SkyBox
{
	static constexpr const char* _vertexPath = "src/shaders/skybox.vert";
	static constexpr const char* _fragPath = "src/shaders/skybox.frag";

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
	void	Render(const CameraData& cam_data);
};
