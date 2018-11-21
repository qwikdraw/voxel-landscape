
#include "Chunk.hpp"
#include "Window.hpp"
#include "FreeCamera.hpp"
#include "FPSDisplay.hpp"
#include "Time.hpp"

std::vector<Chunk> gen_chunks()
{
	Chunk::Init();
	std::vector<Chunk> out;
	for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 5; y++)
		{
			Chunk c(glm::ivec2(x * 64, y * 64));
			c.Load();
			out.push_back(c);
		}
	}
	return out;
}

int	main(void)
{
	GLenum err;

	Window window(1000, 1000, "");
	glClearColor(0.2, 0.25, 0.3, 1);


	FPSDisplay fps;
	FreeCamera cam(window);
	Time clock;

	std::vector<Chunk> chunks = gen_chunks();

	while (!window.ShouldClose())
	{
		if ((err = glGetError()) != GL_NO_ERROR)
			std::cerr << err << std::endl;
		clock.Step();
		window.Clear();
		cam.Update(clock.Delta());
		auto proj = cam.Projection();
		glm::mat4 view = proj.perspective * proj.lookAt;
		Chunk::Render(view, chunks);
		fps.Render();
		glFinish();
		window.Render();
	}
	window.Close();
}
