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
	static glm::vec3 constexpr _basePos = {0, 0, 0};
	static glm::vec3 constexpr _up = {0, 0, 1};
	static glm::vec3 constexpr _forward = {1, 0, 0};

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
