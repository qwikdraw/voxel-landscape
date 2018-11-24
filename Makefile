NAME = voxel
LIST = main \
Window \
Time \
ShadingProgram \
Chunk \
ChunkLoader \
FPSDisplay \
FreeCamera \
Landscape \
range_xor \
SkyBox \
Texture

SRC_DIR = src
OBJ_DIR = obj

SRC = $(addsuffix .cpp, $(addprefix src/, $(LIST)))
OBJ = $(addsuffix .o, $(addprefix $(OBJ_DIR)/, $(LIST)))

CPPFLAGS = -std=c++14 \
$(shell pkg-config --cflags glfw3 glm) \
-I lib/entt/src \
-I lib/lodepng \
-g -O3 -march=native \
-fsanitize=address

LDFLAGS = -framework OpenGl \
$(shell pkg-config --libs glfw3 glm) \
-L lib/lodepng -llodepng \
-fsanitize=address

all: $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "\033[34;1mCompiling: \033[0m"$<
	@clang++ $(CPPFLAGS) -c -o $@ $<

lib/lodepng/liblodepng.a: lib/lodepng/lodepng.cpp
	@echo "\033[35;1mCompiling Dependency: \033[0m"$<
	@clang++ $(CPPFLAGS) -c -o lib/lodepng/lodepng.o $<
	@ar rcs $@ lib/lodepng/lodepng.o

$(NAME): lib/lodepng/liblodepng.a $(OBJ)
	@echo "\033[32;1mLinking.. \033[0m"
	@clang++ $(LDFLAGS) -o $@ $^
	@echo "\033[32;1mCreated:\033[0m "$(NAME)

clean:
	@echo "\033[31;1mCleaning..\033[0m"
	@rm -f $(OBJ)
	@rm -f lib/lodepng/lodepng.o

fclean:
	@echo "\033[31;1mFull Cleaning..\033[0m"
	@rm -rf $(OBJ_DIR)
	@rm -f $(NAME)
	@rm -f lib/lodepng/liblodepng.a

re:	fclean all



deps:
	@./deps.sh

.PHONY: clean fclean all re docs
