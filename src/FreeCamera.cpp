#include <math.h>
#include "FreeCamera.hpp"

FreeCamera::FreeCamera(Window& window, double yaw, double pitch) : _window(window)
{
	_data.position = glm::vec3(0, 128, 0);
	_data.direction = glm::vec3(0, 0, -1);
	_aspect = 1.0;
	_near = 0.1;
	_far = 500.0;
	_fov = 80.0;
	_yaw = yaw;
	_pitch = pitch;

	updateView();
	_data.projection = glm::perspective(glm::radians(_fov), _aspect, _near, _far);
	_data.VP = _data.projection * _data.view;
	_mouse_pos_old = _window.MousePos();
}

void	FreeCamera::updateView(void)
{
	_data.direction = glm::vec3(
		cos(_pitch) * sin(_yaw),
		sin(_pitch),
		cos(_pitch) * cos(_yaw)
	);
	_up = glm::cross(
		glm::vec3(
			sin(_yaw - M_PI / 2.0),
			0.0,
			cos(_yaw - M_PI / 2.0)
		),
		_data.direction
	);
	_data.view = glm::lookAt(
		_data.position,
		_data.position + _data.direction,
		_up
	);
}

void	FreeCamera::Update(double dt)
{
	bool moved = false;

	if (_aspect != _window.GetAspect())
	{
		_aspect = _window.GetAspect();
		_data.projection = glm::perspective(glm::radians(_fov), _aspect, _near, _far);
	}
	float speed = 1.0f;
	if (_window.Key(' '))
		speed *= 20.0f;
	if (_window.Key('W'))
	{
		_data.position += _data.direction * dt * speed;
		moved = true;
	}
	if (_window.Key('S'))
	{
		_data.position -= _data.direction * dt * speed;
		moved = true;
	}
	if (_window.Key('A'))
	{
		_data.position -= glm::cross(_data.direction, _up) * dt * speed;
		moved = true;
	}
	if (_window.Key('D'))
	{
		_data.position += glm::cross(_data.direction, _up) * dt * speed;
		moved = true;
	}
	if (_window.Key('Z'))
	{
		_data.position += _up * dt * speed;
		moved = true;
	}
	if (_window.Key('X'))
	{
		_data.position -= _up * dt * speed;
		moved = true;
	}

	if (_window.MousePos() != _mouse_pos_old)
	{
		glm::vec2 mouse_delta = _mouse_pos_old - _window.MousePos();
		_yaw += mouse_delta.x * dt * 50.0;
		_pitch -= mouse_delta.y * dt * 50.0;

		_mouse_pos_old = _window.MousePos();
		moved = true;
	}

	if (moved)
	{
		updateView();
		_data.VP = _data.projection * _data.view;
	}
}

const CameraData& FreeCamera::GetCameraData(void)
{
	return _data;
}
