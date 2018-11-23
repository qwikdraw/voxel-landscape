
#include "Chunk.hpp"
#include "Window.hpp"
#include "FreeCamera.hpp"
#include "FPSDisplay.hpp"
#include "Time.hpp"
#include "Landscape.hpp"

int	main(void)
{
	GLenum err;

	Window window(1000, 1000, "");
	glClearColor(0.2, 0.25, 0.3, 1);


	FPSDisplay fps;
	FreeCamera cam(window);
	Time clock;

	Chunk::Init();
	Landscape landscape;

	while (!window.ShouldClose())
	{
		if ((err = glGetError()) != GL_NO_ERROR)
			std::cerr << err << std::endl;
		clock.Step();
		window.Clear();
		cam.Update(clock.Delta());
		landscape.Render(cam.Projection());
		fps.Render();
		glFinish();
		window.Render();
	}
	window.Close();
}
