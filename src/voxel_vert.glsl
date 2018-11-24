#version 410 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

uniform mat4 perspective;
uniform mat4 lookAt;
uniform vec2 pos;
uniform vec3 campos;

out	vec3 normal_v;
out vec3 dir_v;
out float dist_v;
out float height_v;
out vec2 uv_v;

void	main()
{
    vec3 vertpos = vertex + vec3(pos.x, 0, pos.y);
    gl_Position = perspective * lookAt * vec4(vertpos, 1);
    normal_v = normal;
    dir_v = vec3(vertpos) - campos;
    dist_v = length(dir_v);
    dir_v = normalize(dir_v);
    height_v = vertex.y;
    uv_v = uv;
}
