
#include "Chunk.hpp"
#include "Window.hpp"
#include "FreeCamera.hpp"
#include "Landscape.hpp"
#include "FPSDisplay.hpp"

static bool	sphere(glm::vec3 p)
{
	if (p.x + p.y + p.z < p.y * p.y - p.z * p.x + 2 * p.y * p.x)
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
	
	while (!window.ShouldClose())
	{
		window.Clear();
		cam.Update();
		l.Render(cam.Projection());
		fps.Render();
		glFinish();
		window.Render();
	}
	window.Close();
}
