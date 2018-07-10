
#include "Chunk.hpp"
#include "Window.hpp"
#include "FreeCamera.hpp"
#include "Landscape.hpp"
#include "FPSDisplay.hpp"

static bool	sphere(glm::vec3 p)
{
	if (glm::length(glm::mod(p, 100.0f) - glm::vec3(50.0f)) < 40.0f)
		return true;
	return false;
}

int	main(void)
{
	Window window(1000, 1000, "");
	
	Landscape l(sphere);
	FPSDisplay fps;

	glClearColor(0.2, 0.25, 0.3, 1);

	FreeCamera cam(window);
	Time clock;
	
	while (!window.ShouldClose())
	{
		clock.Step();
		window.Clear();
		cam.Update(clock.Delta());
		l.Render(cam.Projection());
		fps.Render();
		glFinish();
		window.Render();
	}
	window.Close();
}
