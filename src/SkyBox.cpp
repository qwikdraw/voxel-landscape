#include "SkyBox.hpp"

const std::vector<float> SkyBox::_vertexArray = {
	//back
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	+1.0f, -1.0f, -1.0f,
	+1.0f, -1.0f, -1.0f,
	+1.0f, +1.0f, -1.0f,
	-1.0f, +1.0f, -1.0f,

	//left
	-1.0f, -1.0f, +1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, +1.0f, -1.0f,
	-1.0f, +1.0f, -1.0f,
	-1.0f, +1.0f, +1.0f,
	-1.0f, -1.0f, +1.0f,

	//right
	+1.0f, -1.0f, -1.0f,
	+1.0f, -1.0f, +1.0f,
	+1.0f, +1.0f, +1.0f,
	+1.0f, +1.0f, +1.0f,
	+1.0f, +1.0f, -1.0f,
	+1.0f, -1.0f, -1.0f,

	//front
	-1.0f, -1.0f, +1.0f,
	-1.0f, +1.0f, +1.0f,
	+1.0f, +1.0f, +1.0f,
	+1.0f, +1.0f, +1.0f,
	+1.0f, -1.0f, +1.0f,
	-1.0f, -1.0f, +1.0f,

	//top
	-1.0f,  1.0f, -1.0f,
	+1.0f,  1.0f, -1.0f,
	+1.0f,  1.0f, +1.0f,
	+1.0f,  1.0f, +1.0f,
	-1.0f,  1.0f, +1.0f,
	-1.0f,  1.0f, -1.0f,

	//bot
	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, +1.0f,
	+1.0f, -1.0f, -1.0f,
	+1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, +1.0f,
	+1.0f, -1.0f, +1.0f
};

void SkyBox::_loadArrayBuffers()
{
	glGenBuffers(1, &_vertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexArrayID);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(GLfloat) * _vertexArray.size(),
		_vertexArray.data(),
		GL_STATIC_DRAW
	);
}

void SkyBox::_loadTextures(const std::vector<Texture*>& textures)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &_textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _textureID);

	for (int i = 0; i < 6; i++)
	{
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0,
			GL_RGBA,
			textures[i]->Width(),
			textures[i]->Height(),
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			textures[i]->Data()
		);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void SkyBox::_makeVAO()
{
	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, _vertexArrayID);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}

SkyBox::SkyBox(std::string right, std::string left, std::string top,
			std::string bot, std::string back, std::string front)
{
	_program = new ShadingProgram(_vertexPath, _fragPath);
	_textureLocationID = glGetUniformLocation(_program->ID(), "tex");
	glUseProgram(_program->ID());

	_projectionID = glGetUniformLocation(_program->ID(), "projection");

	_loadArrayBuffers();

	std::vector<Texture*> textures;
	textures.push_back(new Texture(right));
	textures.push_back(new Texture(left));
	textures.push_back(new Texture(top));
	textures.push_back(new Texture(bot));
	textures.push_back(new Texture(back));
	textures.push_back(new Texture(front));
	_loadTextures(textures);
	_makeVAO();
}

SkyBox::~SkyBox(void)
{
	glDeleteTextures(1, &_textureID);
	glDeleteBuffers(1, &_vertexArrayID);
	delete _program;
}

void	SkyBox::Render(const CameraData& cam_data)
{
	_program->Use();
	glm::mat4 transform = cam_data.projection * glm::mat4(glm::mat3(cam_data.view));
	glUniformMatrix4fv(
		_projectionID,
		1,
		GL_FALSE,
		glm::value_ptr(transform)
	);

	glBindTexture(GL_TEXTURE_CUBE_MAP, _textureID);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(_textureLocationID, 0);
	glBindVertexArray(_VAO);

	// save old
	GLint old_cull_face;
	glGetIntegerv(GL_CULL_FACE_MODE, &old_cull_face);
	GLint old_depth;
	glGetIntegerv(GL_DEPTH_FUNC, &old_depth);

	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, _vertexArray.size() / 3);

	// reset to old state
	glCullFace(old_cull_face);
	glDepthFunc(old_depth);
}
