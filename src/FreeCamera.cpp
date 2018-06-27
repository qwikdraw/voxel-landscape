#include "FreeCamera.hpp"

glm::vec3 constexpr FreeCamera::_basePos;
glm::vec3 constexpr FreeCamera::_up;
glm::vec3 constexpr FreeCamera::_forward;

FreeCamera::FreeCamera(Window& window) : _window(window)
{
	_position = _basePos;
	_rotation = glm::mat4(1);
	_aspect = 1;
	_near = 0.1;
	_far = 512;
	_fov = 80;

        glm::mat4 translate = glm::translate(_position);
	glm::mat4 lookAt = glm::lookAt(glm::vec3(translate * _rotation * glm::vec4(_basePos, 1)),
				       glm::vec3(translate * _rotation * glm::vec4(_forward, 1)),
				       glm::vec3(translate * _rotation * glm::vec4(_up, 0)));
	glm::mat4 perspective = glm::perspective(glm::radians(_fov), _aspect, _near, _far);
	_perspective.first = lookAt;
	_perspective.second = perspective;
}

void	FreeCamera::relativeMove(glm::vec3 amount)
{
	glm::vec3 absolute = glm::vec3(_rotation * glm::vec4(amount, 0));
	_position += absolute;
}

void	FreeCamera::Update(void)
{
	bool moved = false;
	
	if (_window.Key('W'))
	{
		relativeMove(_forward);
		moved = true;
	}
	if (_window.Key('S'))
	{
		relativeMove(-_forward);
		moved = true;
	}
	if (_window.Key('A'))
	{
		relativeMove(glm::rotate(_forward, glm::radians(90.0f), _up));
		moved = true;
	}
	if (_window.Key('D'))
	{
		relativeMove(glm::rotate(_forward, glm::radians(-90.0f), _up));
		moved = true;
	}
	if (_window.Key('Z'))
	{
		relativeMove(_up);
		moved = true;
	}
	if (_window.Key('X'))
	{
		relativeMove(-_up);
		moved = true;
	}
	if (_window.Key(GLFW_KEY_LEFT))
	{
		_rotation = glm::rotate(_rotation, glm::radians(1.0f), _up);
		moved = true;
	}
	if (_window.Key(GLFW_KEY_RIGHT))
	{
		_rotation = glm::rotate(_rotation, glm::radians(-1.0f), _up);
		moved = true;
	}
	if (_window.Key(GLFW_KEY_DOWN))
	{
		_rotation = glm::rotate(_rotation, glm::radians(1.0f), glm::vec3(0, 1, 0));
		moved = true;
	}
	if (_window.Key(GLFW_KEY_UP))
	{
		_rotation = glm::rotate(_rotation, glm::radians(-1.0f), glm::vec3(0, 1, 0));
		moved = true;
	}
	
	if (moved)
	{
		glm::mat4 translate = glm::translate(_position);
		glm::mat4 lookAt = glm::lookAt(glm::vec3(translate * glm::vec4(_basePos, 1)),
					       glm::vec3(translate * _rotation * glm::vec4(_forward, 1)),
					       glm::vec3(translate * _rotation * glm::vec4(_up, 0)));
		glm::mat4 perspective = glm::perspective(glm::radians(_fov), _aspect, _near, _far);

		_perspective.first = lookAt;
		_perspective.second = perspective;
	}
}

std::pair<glm::mat4, glm::mat4> FreeCamera::Perspective(void)
{
	return _perspective;
}
