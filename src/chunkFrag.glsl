#version 410 core

in float lightAmount;

out vec3 color;

void	main()
{
	color = vec3(1, 1, 0.5) * lightAmount;
}