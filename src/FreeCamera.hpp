#pragma once

#include "util_inc.hpp"
#include "Window.hpp"

struct	Projection
{
	glm::mat4 lookAt;
	glm::mat4 perspective;
	glm::vec3 position;
	glm::vec3 dir;
};

class	FreeCamera
{
	static glm::vec3 const _basePos;
	static glm::vec3 const _up;
	static glm::vec3 const _forward;

	Window& _window;

	glm::mat4 _rotation;
	float _aspect;
	float _near;
	float _far;
	float _fov;
	Projection _projection;

	void	relativeMove(glm::vec3 amount, double dt);

public:

	FreeCamera(Window& window);

	void	Update(double dt);
	const Projection& Projection(void);
};
