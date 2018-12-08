#pragma once

#include "util_inc.hpp"
#include "ShadingProgram.hpp"
#include "Texture.hpp"

class	Text
{
private:
	static constexpr const char* _fontFile = "assets/textures/game_font.png";
	static constexpr const char* _vertexPath = "src/shaders/text.vert";
    static constexpr const char* _fragPath = "src/shaders/text.frag";

	static ShadingProgram *_program;

	std::string _message;

	static std::vector<float> _square;
	static std::vector<float> _uv;

	static GLuint _squareID;
	static GLuint _UVID;
	static GLuint _textureID;
	static GLuint _textureLocationID;
	static GLuint _VAO;

	static bool _init;

	void	RenderChar(char c, glm::vec2 topleft, glm::vec2 botright);

public:

	Text(std::string message);

	void	Render(float aspect);
};
