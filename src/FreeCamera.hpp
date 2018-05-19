#pragma once

#include "voxel.hpp"
#include "Window.hpp"

class	FreeCamera
{
	static glm::vec3 constexpr _basePos = {0, 0, 0};
	static glm::vec3 constexpr _up = {0, 0, 1};
	static glm::vec3 constexpr _forward = {1, 0, 0};

	Window& _window;
	
	glm::vec3 _position;
	glm::mat4 _rotation;
	float _aspect;
	float _near;
	float _far;
	float _fov;
	std::pair<glm::mat4, glm::mat4> _perspective;

	void	relativeMove(glm::vec3 amount);
	
public:

	FreeCamera(Window& window);
	
	void	Update(void);
	std::pair<glm::mat4, glm::mat4> Perspective(void);
};
