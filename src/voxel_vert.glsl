#version 410 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

uniform mat4 view;
uniform vec2 pos;

out	vec3 normal_v;
out vec2 uv_v;

void	main()
{
    gl_Position = view * vec4(vertex + vec3(pos.x, 0, pos.y), 1);
    normal_v = vec3(view * vec4(normal, 0));
    uv_v = uv;
}
