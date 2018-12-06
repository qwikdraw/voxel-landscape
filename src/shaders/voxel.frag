#version 410 core

in vec3 normal_v;
in vec3 dir_v;
in float dist_v;
in float height_v;
in vec2 uv_v;

uniform sampler2D tex;

out vec3 color;

void	main()
{
	float modify = max(dot(normalize(normal_v), dir_v), 0.05);
	modify /= dist_v;
	modify = (modify / (modify + 0.015)) * (height_v / 128.0);
	color = texture(tex, uv_v).rgb * modify;
}
