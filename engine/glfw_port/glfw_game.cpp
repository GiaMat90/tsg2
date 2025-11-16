#ifdef GLFW_GAME

#include "glfw_game.h"
#include "gl_includes.h"

glfw_game::glfw_game() {
	init();
}

glfw_game::~glfw_game() {
	quit();
}

bool glfw_game::init() {
	return GLFW_TRUE == glfwInit();
}

void glfw_game::quit() {
	glfwTerminate();
}
#endif