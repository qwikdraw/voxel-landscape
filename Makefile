
NAME = voxel

SRC = src/*.cpp

FRAMEWORKS = -framework OpenGl

GLFW_INC = -I ~/.brew/include

GLFW_LINK = -L ~/.brew/lib -lglfw

MY_INC = -I src/.

FLAGS = -std=c++17 -O3 -fsanitize=undefined -fsanitize=address

make:
	g++ -g $(FLAGS) $(SRC) -o $(NAME) $(FRAMEWORKS) $(MY_INC) $(GLFW_INC) $(GLFW_LINK)

fclean:
	rm $(NAME)
