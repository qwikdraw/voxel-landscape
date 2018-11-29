#version 410 core

layout (location = 0) in vec3 vertex;

out vec3 texCoor;

uniform mat4 projection;

void	main()
{
	vec4 transformed_pos = projection * vec4(vertex, 1.0);
	gl_Position = transformed_pos.xyww;
	texCoor = vertex;
}
