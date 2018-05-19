#version 410 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 pos;
layout(location = 3) in float size;

uniform mat4 transform;
uniform mat4 lookAt;
uniform mat4 projection;

out	float lightAmount;
out	vec3 col;

void	main()
{
	vec3 trueVertex = vertex * size + pos;
	mat4 L = lookAt;

	vec4 ray = L * vec4(trueVertex, 1);
	ray.w = 0;

	lightAmount = max( abs( dot(L * vec4(normal, 0), normalize(ray)) ), 0.2);

	col = normalize(abs(trueVertex));

	mat4 T = projection * L;

	gl_Position = T * vec4(trueVertex, 1);
}