#include "Chunk.hpp"

ShadingProgram* Chunk::_program;
GLuint Chunk::_perspectiveID;
GLuint Chunk::_lookAtID;
GLuint Chunk::_posID;
GLuint Chunk::_texID;
GLuint Chunk::_texLocID;

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

land_map_t terrain_gen(glm::ivec2 pos)
{
    // 34 because 32x32 grid + border on index 0 and 33
    float noise_1[34][34];
    float noise_2[34][34];

    for (size_t x = 0; x < 34; x++)
    {
        for (size_t z = 0; z < 34; z++)
        {
            noise_1[x][z] = glm::perlin(glm::vec2(glm::ivec2(x, z) + pos) / 133.37);
            noise_2[x][z] = glm::perlin(glm::vec2(glm::ivec2(x, z) + pos +
                                        glm::ivec2(444, 124) /* random offset */) / 10.37);
        }
    }
    land_map_t landmap;
    for (size_t x = 0; x < 34; x++)
        for (size_t z = 0; z < 34; z++)
            landmap[x][z] = section_gen((noise_1[x][z] + 1.0) / 2.0, (noise_2[x][z] + 1.0) / 2.0);
    return landmap;
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
    // this entire function will be replaced when we get actual texture loading
    // but for now it should display a checkerboard
    size_t width = 2;
    size_t height = 2;
    unsigned char data[] = {90, 130, 200, 255, 90, 130, 200, 255,
                            90, 130, 200, 255, 90, 130, 200, 255,};

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &_texID);
	glBindTexture(GL_TEXTURE_2D, _texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D,
		     0,
		     GL_RGBA,
		     width,
		     height,
		     0,
		     GL_RGBA,
		     GL_UNSIGNED_BYTE,
		     data);
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

void Chunk::_addRectangle(glm::vec3 center, glm::vec3 height, glm::vec3 width)
{
    if (glm::length(height) == 0 || glm::length(width) == 0)
        throw std::runtime_error("width or height should not be 0");

    glm::vec3 corner1 = center + height / 2.0 + width / 2.0;
    glm::vec3 corner2 = center - height / 2.0 + width / 2.0;
    glm::vec3 corner3 = center - height / 2.0 - width / 2.0;
    glm::vec3 corner4 = center + height / 2.0 - width / 2.0;

    glm::vec3 normal = glm::cross(height, width);

    glm::vec2 uv1 = glm::vec2(1, 1);
    glm::vec2 uv2 = glm::vec2(1, 0);
    glm::vec2 uv3 = glm::vec2(0, 0);
    glm::vec2 uv4 = glm::vec2(0, 1);

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

void Chunk::_addSection(float x, float z,
    land_section_t right,
    land_section_t left,
    land_section_t front,
    land_section_t back,
    land_section_t main)
{
    for (auto col : main)
    {
        // top face
        _addRectangle(glm::vec3(x, col.top, z),
                    glm::vec3(0, 0, -1),
                    glm::vec3(1, 0, 0));
        // bot face
        _addRectangle(glm::vec3(x, col.bot, z),
                    glm::vec3(0, 0, 1),
                    glm::vec3(1, 0, 0));
    }
    for (auto col : front)
    {
        // front face
        _addRectangle(glm::vec3(x, (col.bot + col.top) / 2.0, z + 0.5),
                    glm::vec3(-1, 0, 0),
                    glm::vec3(0, col.top - col.bot, 0));
    }
    for (auto col : back)
    {
        // back face
        _addRectangle(glm::vec3(x, (col.bot + col.top) / 2.0, z - 0.5),
                    glm::vec3(1, 0, 0),
                    glm::vec3(0, col.top - col.bot, 0));
    }
    for (auto col : right)
    {
        // right face
        _addRectangle(glm::vec3(x + 0.5, (col.bot + col.top) / 2.0, z),
                    glm::vec3(0, 0, 1),
                    glm::vec3(0, col.top - col.bot, 0));
    }
    for (auto col : left)
    {
        // left face
        _addRectangle(glm::vec3(x - 0.5, (col.bot + col.top) / 2.0, z),
                    glm::vec3(0, 0, -1),
                    glm::vec3(0, col.top - col.bot, 0));
    }
}

void Chunk::_createMesh(const land_map_t& map)
{
    land_map_t left;
    land_map_t right;
    land_map_t front;
    land_map_t back;

    for (size_t x = 1; x < 34; x++)
    {
        for (size_t z = 1; z < 34; z++)
        {
                auto p1 = range_xor(map[x - 1][z], map[x][z]);
                auto p2 = range_xor(map[x][z - 1], map[x][z]);

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
            // subtract 12.5 becasue cube centers range from -15.5 to 15.5
            _addSection(float(x) - 16.5, float(z) - 16.5,
                right[x][z], left[x][z], front[x][z], back[x][z], map[x][z]);
        }
    }
}

Chunk::Chunk(glm::ivec2 pos)
{
    _pos = pos;
    land_map_t map = terrain_gen(pos);
    _createMesh(map);
}

void Chunk::Unload()
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
    _posID = glGetUniformLocation(_program->ID(), "pos");
    _texLocID = glGetUniformLocation(_program->ID(), "tex");
}

void Chunk::Load()
{
    _loadArrayBuffers();
    _loadTexture();
    _makeVAO();
}

glm::ivec2 Chunk::Pos()
{
    return _pos;
}

void Chunk::Render(const Projection& projection, const std::vector<Chunk*>& chunks)
{
    GLenum err;

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
        glUniformMatrix4fv(_perspectiveID, 1, GL_FALSE,
            glm::value_ptr(projection.perspective));
        glUniformMatrix4fv(_lookAtID, 1, GL_FALSE,
            glm::value_ptr(projection.lookAt));
        glDrawArrays(GL_TRIANGLES, 0, chunk->_triangles.size() * 3);
    }
}
