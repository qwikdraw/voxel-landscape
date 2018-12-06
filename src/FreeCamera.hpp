#pragma once

#include "util_inc.hpp"
#include "Window.hpp"

struct	CameraData
{
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 VP;
	glm::vec3 position;
	glm::vec3 direction;
};

class	FreeCamera
{
	static glm::vec3 const _forward;

	Window& _window;
	glm::vec2 _mouse_pos_old;
	float _aspect;
	float _near;
	float _far;
	float _fov;

	float _yaw;
	float _pitch;
	glm::vec3 _up;

	CameraData _data;

	void	relativeMove(glm::vec3 amount, double dt);
	void	updateView(void);

public:
	FreeCamera(Window& window, double yaw=0.0, double pitch=0.0);
	void	Update(double dt);
	const	CameraData& GetCameraData(void);
};
