#pragma once

#include "ShadingProgram.hpp"
#include "util_inc.hpp"

// defines a 1 x (top - bot) x 1 cuboid with undefined x, z locations
struct Column
{
    int bot;
    int top;
};

// each pair specifies a start and end height for the land.
// follows following rules:
// vector[i].bot > vector[i - 1].top
// vector[i].top > vector[i].bot
// vector[i].top <= 256
// vector[i].bot >= 0
typedef std::vector<Column> land_section_t;

// The size is 66 because a chunk is 64x64 and we need 1 block of
// context around the edges (a border) to generate the chunk
typedef std::array<std::array<land_section_t, 66>, 66> land_map_t;

land_map_t terrain_gen(glm::ivec2 pos);

// a chunk has dimensions of 64x64x256
class Chunk
{
    static constexpr const char* _vertexPath = "src/voxel_vert.glsl";
    static constexpr const char* _fragPath = "src/voxel_frag.glsl";

    static ShadingProgram *_program;
    static GLuint _viewID;
    static GLuint _posID;
    static GLuint _texID;
    static GLuint _texLocID;

    // array of vertices
    std::vector<glm::vec3> _triangles;

    // 1 uv per vertex
    std::vector<glm::vec2> _uvs;

    // 1 normal per vertex
    std::vector<glm::vec3> _normals;

    // IDs to the OpenGL array buffers
    GLuint _trianglesID;
    GLuint _uvsID;
    GLuint _normalsID;

    // vertex array object
    GLuint _VAO;

    glm::ivec2 _pos;

    void _loadArrayBuffers();
    void _loadTexture();
    void _makeVAO();

    // adds rectangle to the _triangles, _uvs, _normals.
    // dot(height, width) == 0.
    void _addRectangle(glm::vec3 center, glm::vec3 height, glm::vec3 width);

    // creates a naive mesh for the column
    void _columnToMesh(Column, float x, float z);

    // fill the _triangles, _uvs and _normals
    void _createMesh(const land_map_t&);

public:

    // creates a chunk that spans: pos - vec2(32) to pos + vec2(32)
    // and has height from 0 - 256
    // contains no OpenGL calls and so can be called on any thread
    Chunk(glm::ivec2 pos);

    // call this method to initialize shading program and potentially
    // other things in future
    static void Init();

    // the stuff that should have been in the constructor but contained
    // OpenGL functions and so didn't allow creation of Chunk object in
    // seperate threads. Call Load() only in the main thread.
    void Load();

    // must be called on main thread.
    void Unload();

    // render vector of chunks with view matrix (projection * lookat)
    static void Render(const glm::mat4& view, const std::vector<Chunk>&);
};
