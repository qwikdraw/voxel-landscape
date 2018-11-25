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
Texture \
Text

SRC_DIR = src
OBJ_DIR = obj

SRC = $(addsuffix .cpp, $(addprefix src/, $(LIST)))
OBJ = $(addsuffix .o, $(addprefix $(OBJ_DIR)/, $(LIST)))
DEP = $(OBJ:%.o=%.d)

MAKEFLAGS=-j4

CPPFLAGS = -std=c++17 -Wall -Wextra -Werror -Wno-unused-parameter \
$(shell pkg-config --cflags glfw3 glm) \
-I lib/lodepng \
-g -O3 -march=native \
#-fsanitize=address -fsanitize=undefined

LDFLAGS = -framework OpenGl \
$(shell pkg-config --libs glfw3 glm) \
-L lib/lodepng -llodepng -pipe -flto=thin \
#-fsanitize=address -fsanitize=undefined

all: $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

-include $(DEP)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@printf "\e[34;1mCompiling: \e[0m%s\n" $<
	@clang++ $(CPPFLAGS) -MMD -c $< -o $@

lib/lodepng/liblodepng.a: lib/lodepng/lodepng.cpp
	@printf "\e[35;1mCompiling Dependency: \e[0m%s\n" $<
	@clang++ $(CPPFLAGS) -pipe -flto=thin -c -o lib/lodepng/lodepng.o $<
	@ar rcs $@ lib/lodepng/lodepng.o

$(NAME): lib/lodepng/liblodepng.a $(OBJ)
	@echo "\033[32;1mLinking.. \033[0m"
	@clang++ $(LDFLAGS) -o $@ $^
	@echo "\033[32;1mCreated:\033[0m "$(NAME)

clean:
	@printf "\e[31;1mCleaning..\e[0m\n"
	@rm -f $(OBJ)
	@rm -f lib/lodepng/lodepng.o

fclean:
	@printf "\e[31;1mFull Cleaning..\e[0m\n"
	@rm -rf $(OBJ_DIR)
	@rm -f $(NAME)
	@rm -f lib/lodepng/liblodepng.a

re:
	@$(MAKE) fclean 2>/dev/null
	@$(MAKE) 2>/dev/null

deps:
	@./deps.sh

.PHONY: clean fclean all re
