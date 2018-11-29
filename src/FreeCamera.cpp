#include "FreeCamera.hpp"

glm::vec3 const FreeCamera::_up = {0, 1, 0};

FreeCamera::FreeCamera(Window& window) : _window(window)
{
	_uniforms.position = glm::vec3(0, 128, 0);
	_uniforms.direction = glm::vec3(0, 0, -1);
	_aspect = 1;
	_near = 0.1;
	_far = 500;
	_fov = 80;

	updateView();
	_uniforms.projection = glm::perspective(glm::radians(_fov), _aspect, _near, _far);
	_uniforms.VP = _uniforms.projection * _uniforms.view;
	_mouse_pos_old = _window.MousePos();
}

void	FreeCamera::updateView(void)
{
	_uniforms.view = glm::lookAt(
		_uniforms.position,
		_uniforms.position + _uniforms.direction,
		_up
	);
}

void	FreeCamera::Update(double dt)
{
	bool moved = false;

	if (_aspect != _window.GetAspect())
	{
		_uniforms.projection = glm::perspective(glm::radians(_fov), _aspect, _near, _far);
	}
	if (_window.Key('W'))
	{
		_uniforms.position += _uniforms.direction * 50 * dt;
		moved = true;
	}
	if (_window.Key('S'))
	{
		_uniforms.position -= _uniforms.direction * 50 * dt;
		moved = true;
	}
	if (_window.Key('A'))
	{
		_uniforms.position -= glm::cross(_uniforms.direction, _up) * 50 * dt;
		moved = true;
	}
	if (_window.Key('D'))
	{
		_uniforms.position += glm::cross(_uniforms.direction, _up) * 50 * dt;
		moved = true;
	}
	if (_window.Key('Z'))
	{
		_uniforms.position += _up * 50 * dt;
		moved = true;
	}
	if (_window.Key('X'))
	{
		_uniforms.position -= _up * 50 * dt;
		moved = true;
	}

	if (_window.MousePos() != _mouse_pos_old)
	{
		glm::vec2 mouse_delta = _mouse_pos_old - _window.MousePos();
		_uniforms.direction = glm::rotate(_uniforms.direction, mouse_delta.x, glm::vec3(0, 1, 0));
		_uniforms.direction = glm::rotate(_uniforms.direction, mouse_delta.y, -glm::cross(_uniforms.direction, _up));
		_mouse_pos_old = _window.MousePos();
		moved = true;
	}

	if (moved)
	{
		updateView();
		_uniforms.VP = _uniforms.projection * _uniforms.view;
	}
}

const CameraUniforms& FreeCamera::GetUniforms(void)
{
	return _uniforms;
}
