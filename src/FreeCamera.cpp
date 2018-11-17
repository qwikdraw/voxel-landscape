#include "FreeCamera.hpp"

glm::vec3 constexpr FreeCamera::_basePos;
glm::vec3 constexpr FreeCamera::_up;
glm::vec3 constexpr FreeCamera::_forward;

FreeCamera::FreeCamera(Window& window) : _window(window)
{
	_projection.position = _basePos;
	_rotation = glm::mat4(1);
	_aspect = 1;
	_near = 0.1;
	_far = 512;
	_fov = 80;

    glm::mat4 translate = glm::translate(_projection.position);
	_projection.lookAt = glm::lookAt(glm::vec3(translate * _rotation * glm::vec4(_basePos, 1)),
				       glm::vec3(translate * _rotation * glm::vec4(_forward, 1)),
				       glm::vec3(translate * _rotation * glm::vec4(_up, 0)));
	_projection.perspective = glm::perspective(glm::radians(_fov), _aspect, _near, _far);
	_projection.dir = glm::vec3(_rotation * glm::vec4(_forward, 0));
}

void	FreeCamera::relativeMove(glm::vec3 amount, double dt)
{
	glm::vec3 absolute = glm::vec3(_rotation * glm::vec4(amount, 0));
	_projection.position += absolute * dt * 10;
}

void	FreeCamera::Update(double dt)
{
	std::cout << "cam pos: ";
	std::cout << _projection.position.x << " " << _projection.position.y << " " << _projection.position.z
		  << std::endl;
	bool moved = false;

	_aspect = _window.GetAspect();
	if (_window.Key('W'))
	{
		relativeMove(_forward, dt);
		moved = true;
	}
	if (_window.Key('S'))
	{
		relativeMove(-_forward, dt);
		moved = true;
	}
	if (_window.Key('A'))
	{
		relativeMove(glm::rotate(_forward, glm::radians(90.0f), _up), dt);
		moved = true;
	}
	if (_window.Key('D'))
	{
		relativeMove(glm::rotate(_forward, glm::radians(-90.0f), _up), dt);
		moved = true;
	}
	if (_window.Key('Z'))
	{
		relativeMove(_up, dt);
		moved = true;
	}
	if (_window.Key('X'))
	{
		relativeMove(-_up, dt);
		moved = true;
	}
	if (_window.Key(GLFW_KEY_LEFT))
	{
		_rotation = glm::rotate(_rotation, (float)glm::radians(90.0 * dt), _up);
		moved = true;
	}
	if (_window.Key(GLFW_KEY_RIGHT))
	{
		_rotation = glm::rotate(_rotation, (float)glm::radians(-90.0 * dt), _up);
		moved = true;
	}
	if (_window.Key(GLFW_KEY_DOWN))
	{
		_rotation = glm::rotate(_rotation, (float)glm::radians(90.0 * dt), glm::vec3(0, 1, 0));
		moved = true;
	}
	if (_window.Key(GLFW_KEY_UP))
	{
		_rotation = glm::rotate(_rotation, (float)glm::radians(-90.0 * dt), glm::vec3(0, 1, 0));
		moved = true;
	}

	if (moved)
	{
		glm::mat4 translate = glm::translate(_projection.position);
		_projection.lookAt = glm::lookAt(glm::vec3(translate * glm::vec4(_basePos, 1)),
					       glm::vec3(translate * _rotation * glm::vec4(_forward, 1)),
					       glm::vec3(translate * _rotation * glm::vec4(_up, 0)));
		_projection.perspective = glm::perspective(glm::radians(_fov), _aspect, _near, _far);
		_projection.dir	= glm::vec3(_rotation * glm::vec4(_forward, 0));
	}
}

const Projection& FreeCamera::Projection(void)
{
	return _projection;
}
