#pragma once

#include "util_inc.hpp"
#include "Window.hpp"

struct	CameraUniforms
{
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 VP;
	glm::vec3 position;
	glm::vec3 direction;
};

class	FreeCamera
{
	static glm::vec3 const _up;
	static glm::vec3 const _forward;

	Window& _window;
	glm::vec2 _mouse_pos_old;
	float _aspect;
	float _near;
	float _far;
	float _fov;

	CameraUniforms _uniforms;

	void	relativeMove(glm::vec3 amount, double dt);
	void	updateView(void);

public:
	FreeCamera(Window& window);
	void	Update(double dt);
	const	CameraUniforms& GetUniforms(void);
};
