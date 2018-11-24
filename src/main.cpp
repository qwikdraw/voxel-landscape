
#include "Chunk.hpp"
#include "Window.hpp"
#include "FreeCamera.hpp"
#include "FPSDisplay.hpp"
#include "Time.hpp"
#include "Landscape.hpp"
#include "SkyBox.hpp"

int	main(void)
{
	GLenum err;

	Window window(1000, 1000, "");
	glClearColor(0.2, 0.25, 0.3, 1);


	FPSDisplay fps;
	FreeCamera cam(window);
	Time clock;
	SkyBox sky("assets/textures/skybox/right.png",
				"assets/textures/skybox/left.png",
				"assets/textures/skybox/top.png",
				"assets/textures/skybox/bottom.png",
				"assets/textures/skybox/front.png",
				"assets/textures/skybox/back.png");

	Chunk::Init();
	Landscape landscape;

	while (!window.ShouldClose())
	{
		if ((err = glGetError()) != GL_NO_ERROR)
			std::cerr << err << std::endl;
		clock.Step();
		window.Clear();
		cam.Update(clock.Delta());
		sky.Render(cam.Projection());
		landscape.Render(cam.Projection());
		fps.Render();
		glFinish();
		window.Render();
	}
	window.Close();
}
