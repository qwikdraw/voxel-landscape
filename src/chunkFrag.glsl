#version 410 core

in float lightAmount;
in vec3 col;

out vec3 color;

void	main()
{
	color = col * lightAmount;
}