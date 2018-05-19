
#include "Chunk.hpp"
#include "Window.hpp"
#include "FreeCamera.hpp"
#include "Landscape.hpp"

static bool	sphere(glm::vec3 p)
{
	if (glm::length(p) < 70)
		return true;
	return true;
}

int	main(void)
{
	Window window(1000, 1000, "");
	
	OcTree *tree = OcTree::Generate64(sphere, glm::vec3(0, 0, 0));
	Chunk *chunk = new Chunk(glm::vec3(0, 0, 0), tree, 6);

	glClearColor(0.2, 0.25, 0.3, 1);

	FreeCamera cam(window);
	
	while (!window.ShouldClose())
	{
		window.Clear();
		cam.Update();
		chunk->UsePerspective(cam.Perspective());
		chunk->Render();
		window.Render();
	}
	window.Close();
}
