#version 410 core

in vec3 texCoor;

out vec3 color;

uniform samplerCube tex;

void	main()
{
	color = texture(tex, texCoor).rgb;
}
