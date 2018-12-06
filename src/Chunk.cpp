#include "Chunk.hpp"
#include "Texture.hpp"
#include <stdlib.h>

ShadingProgram* Chunk::_program;
GLuint Chunk::_perspectiveID;
GLuint Chunk::_lookAtID;
GLuint Chunk::_posID;
GLuint Chunk::_VPID;
GLuint Chunk::_camposID;
GLuint Chunk::_texID;
GLuint Chunk::_texLocID;

#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF

// 0 <= a, b <= 1
static land_section_t section_gen(float a, float b)
{
	if (a < 0 || b < 0 || a > 1 || b > 1)
		throw std::runtime_error("input must be between 0 and 1");

	float height = 100 + 156 * a;
	float cavern_begin = 10 + 128 * a;
	float cavern_end = 256 * b;

	land_section_t out;

	// if there is no cavern
	if (int(cavern_end) <= int(cavern_begin))
		out.push_back(Column{0, int(height)});
	else
	{
		out.push_back(Column{0, int(cavern_begin)});
		if (int(cavern_end) < int(height))
			out.push_back(Column{int(cavern_end), int(height)});
	}
	return out;
}

land_map_t* terrain_gen(glm::ivec2 pos)
{
	// 34 because 32x32 grid + border on index 0 and 33
	float noise_1;
	float noise_2;
	land_map_t* landmap = new land_map_t;

	for (size_t x = 0; x < 34; x++)
	{
		for (size_t z = 0; z < 34; z++)
		{
			noise_1 = (glm::perlin(
				glm::vec2(glm::ivec2(x, z) + pos) / 93.37f
			) + 1.0f) / 2.0f;
			noise_2 = (glm::simplex(
				glm::vec2(glm::ivec2(x, z) + pos +
				glm::ivec2(444, 124)) / 75.37f
			) + 1.0f) / 2.0f;
			(*landmap)[x][z] = section_gen(noise_1, noise_2);
		}
	}
	return landmap;
}

static BLOCK get_biome(int x, int z)
{

	float temperature = (glm::perlin(
		glm::vec2(glm::ivec2(x, z) + glm::ivec2(-564, 724)) / 130.37
	) + 1.0f) / 2.0f;

	float humidity = (glm::simplex(
		glm::vec2(glm::ivec2(x, z) + glm::ivec2(444, -124)) / 150.37
	) + 1.0f) / 2.0f;

	if (temperature < 0.2)
	{
		if (humidity > 0.6)
			return BLOCK::SNOW;
		else
			return BLOCK::BEDROCK;
	}
	else if (temperature < 0.7)
		return BLOCK::GRASS;
	else
		return BLOCK::SAND;
}

static float get_variance(int x, int y)
{
	return glm::simplex(
		glm::vec2(glm::ivec2(x, y) +
		glm::ivec2(68, 1243)) / 8.37
	);
}

Chunk::Chunk(glm::ivec2 pos)
{
	_pos = pos;
	land_map_t* map = terrain_gen(pos);
	_createMesh(map);
	delete map;
}

void Chunk::Unload(void)
{
	glDeleteBuffers(1, &_trianglesID);
	glDeleteBuffers(1, &_uvsID);
	glDeleteBuffers(1, &_normalsID);
	glDeleteVertexArrays(1, &_VAO);
}

void Chunk::Init()
{
	_program = new ShadingProgram(_vertexPath, _fragPath);
	_perspectiveID = glGetUniformLocation(_program->ID(), "perspective");
	_lookAtID = glGetUniformLocation(_program->ID(), "lookAt");
	_VPID = glGetUniformLocation(_program->ID(), "VP");
	_posID = glGetUniformLocation(_program->ID(), "pos");
	_camposID = glGetUniformLocation(_program->ID(), "campos");
	_texLocID = glGetUniformLocation(_program->ID(), "tex");
	_texID = 0;
	_loadTexture();
}

void Chunk::Load()
{
	_loadArrayBuffers();
	_makeVAO();
}

void Chunk::_loadArrayBuffers()
{
	glGenBuffers(1, &_trianglesID);
	glGenBuffers(1, &_uvsID);
	glGenBuffers(1, &_normalsID);

	glBindBuffer(GL_ARRAY_BUFFER, _trianglesID);
	glBufferData(GL_ARRAY_BUFFER,
		_triangles.size() * 3 * sizeof(GLfloat),
		_triangles.data(),
		GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _uvsID);
	glBufferData(GL_ARRAY_BUFFER,
		_uvs.size() * 2 * sizeof(GLfloat),
		_uvs.data(),
		GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _normalsID);
	glBufferData(GL_ARRAY_BUFFER,
		_normals.size() * 3 * sizeof(GLfloat),
		_normals.data(),
		GL_STATIC_DRAW);
}

void Chunk::_loadTexture()
{
	Texture texure_atlas("assets/textures/texture_new.png");

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &_texID);
	glBindTexture(GL_TEXTURE_2D, _texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	float aniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		texure_atlas.Width(),
		texure_atlas.Height(),
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		texure_atlas.Data()
	);
}

void Chunk::_makeVAO()
{
	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _trianglesID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, _uvsID);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, _normalsID);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}

void Chunk::_addRectangle(glm::vec3 center, glm::vec3 height, glm::vec3 width, unsigned tex_num)
{
	if (glm::length(height) == 0 || glm::length(width) == 0)
		throw std::runtime_error("width or height should not be 0");

	glm::vec3 corner1 = center + (height / 2.0) + (width / 2.0);
	glm::vec3 corner2 = center - (height / 2.0) + (width / 2.0);
	glm::vec3 corner3 = center - (height / 2.0) - (width / 2.0);
	glm::vec3 corner4 = center + (height / 2.0) - (width / 2.0);

	glm::vec3 normal = glm::cross(height, width);

	glm::vec2 uv1;
	glm::vec2 uv2;
	glm::vec2 uv3;
	glm::vec2 uv4;

	if (fabs(normal[1]) == 1.0)
	{
		uv1 = glm::vec2(1.0 / _tex_atlas_width, 1);
		uv2 = glm::vec2(1.0 / _tex_atlas_width, 0);
		uv3 = glm::vec2(0, 0);
		uv4 = glm::vec2(0, 1);
	}
	else
	{
		uv1 = glm::vec2(1.0 / _tex_atlas_width, height[1]);
		uv2 = glm::vec2(1.0 / _tex_atlas_width, 0);
		uv3 = glm::vec2(0, 0);
		uv4 = glm::vec2(0, height[1]);
	}

	float add = (1.0 / double(_tex_atlas_width)) * tex_num;
	uv1.x += add;
	uv2.x += add;
	uv3.x += add;
	uv4.x += add;

	// triangle 1
	_triangles.push_back(corner1);
	_triangles.push_back(corner2);
	_triangles.push_back(corner3);

	_normals.push_back(normal);
	_normals.push_back(normal);
	_normals.push_back(normal);

	_uvs.push_back(uv1);
	_uvs.push_back(uv2);
	_uvs.push_back(uv3);

	// triangle 2
	_triangles.push_back(corner3);
	_triangles.push_back(corner4);
	_triangles.push_back(corner1);

	_normals.push_back(normal);
	_normals.push_back(normal);
	_normals.push_back(normal);

	_uvs.push_back(uv3);
	_uvs.push_back(uv4);
	_uvs.push_back(uv1);
}

static BLOCK get_side(BLOCK side, struct Column col, int top)
{
	int height = col.top - col.bot;

	if (col.bot < 120)
		return BLOCK::STONE;
	else if (col.top == top && height < 4)
		return side;
	else
		return BLOCK::DIRT;
}

void Chunk::_addSection(float x, float z,
	land_section_t right,
	land_section_t left,
	land_section_t front,
	land_section_t back,
	land_section_t main)
{

	int variance = get_variance(_pos.x + x, _pos.y + z) * 5;
	BLOCK biome;
	int top = 0; // Top of column
	for (auto col : main)
		top = std::max(col.top, top);
	if (top < 120 + variance)
		biome = BLOCK::STONE;
	else
		biome = get_biome(_pos.x + x, _pos.y + z);
	for (auto col : main)
	{
		// top face
		BLOCK face_type;
		if (col.top > 210 + variance)
			face_type = BLOCK::SNOW;
		else if (col.top < 120)
			face_type = BLOCK::STONE;
		else
			face_type = biome;
		_addRectangle(
			glm::vec3(x, col.top, z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 0, 1),
			face_type
		);
		// bot face
		_addRectangle(
			glm::vec3(x, col.bot, z),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 0, -1),
			(col.bot < 120 ? BLOCK::STONE : BLOCK::DIRT)
		);
	}
	BLOCK side = biome;
	if (top > 220 + (2 * (variance - 1.0)))
		side = BLOCK::SNOW;
	else if (top > 215 + (2 * (variance - 1.0)))
		side = BLOCK::STONE;
	for (auto col : front)
	{
		// front face
		_addRectangle(
			glm::vec3(x, (col.bot + col.top) / 2.0, z + 0.5),
			glm::vec3(0, col.top - col.bot, 0),
			glm::vec3(1, 0, 0),
			get_side(side, col, top)
		);
	}
	for (auto col : back)
	{
		// back face
		_addRectangle(
			glm::vec3(x, (col.bot + col.top) / 2.0, z - 0.5),
			glm::vec3(0, col.top - col.bot, 0),
			glm::vec3(-1, 0, 0),
			get_side(side, col, top)
		);
	}
	for (auto col : right)
	{
		// right face
		_addRectangle(
			glm::vec3(x + 0.5, (col.bot + col.top) / 2.0, z),
			glm::vec3(0, col.top - col.bot, 0),
			glm::vec3(0, 0, -1),
			get_side(side, col, top)
		);
	}
	for (auto col : left)
	{
		// left face
		_addRectangle(
			glm::vec3(x - 0.5, (col.bot + col.top) / 2.0, z),
			glm::vec3(0, col.top - col.bot, 0),
			glm::vec3(0, 0, 1),
			get_side(side, col, top)
		);
	}
}

void Chunk::_createMesh(const land_map_t* map)
{
	land_map_t left;
	land_map_t right;
	land_map_t front;
	land_map_t back;

	for (size_t x = 1; x < 34; x++)
	{
		for (size_t z = 1; z < 34; z++)
		{
			auto p1 = range_xor((*map)[x - 1][z], (*map)[x][z]);
			auto p2 = range_xor((*map)[x][z - 1], (*map)[x][z]);

			right[x - 1][z] = p1.first;
			left[x][z] = p1.second;
			front[x][z - 1] = p2.first;
			back[x][z] = p2.second;
		}
	}

	// 1 to 33 because we ignore border
	for (size_t x = 1; x < 33; x++)
	{
		for (size_t z = 1; z < 33; z++)
		{
			// subtract 16.5 because cube centers range from -16.5 to 16.5
			_addSection(
				float(x) - 16.5f, float(z) - 16.5f,
				right[x][z], left[x][z], front[x][z], back[x][z], (*map)[x][z]
			);
		}
	}
}

glm::ivec2 Chunk::Pos()
{
	return _pos;
}

void Chunk::Render(const CameraData& cam_data, const std::vector<Chunk*>& chunks)
{

	_program->Use();
	glBindTexture(GL_TEXTURE_2D, _texID);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(_texLocID, 0);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	for (auto chunk : chunks)
	{
		glBindVertexArray(chunk->_VAO);
		glUniform2f(_posID, float(chunk->_pos.x), float(chunk->_pos.y));
		glUniform3fv(_camposID, 1, &cam_data.position[0]);

		glUniformMatrix4fv(_VPID, 1, GL_FALSE,
			glm::value_ptr(cam_data.VP));
		glDrawArrays(GL_TRIANGLES, 0, chunk->_triangles.size() * 3);
	}
	glDisable(GL_CULL_FACE);
}
