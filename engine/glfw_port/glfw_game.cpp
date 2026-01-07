#ifdef GLFW_GAME

#include "glfw_game.h"
#include "gl_includes.h"
// tsg includes
#include <tsg/logger.h>
#include <tsg/io.h>
// std includes
#include <cassert>

static void error_callback(int error, const char* description) {
	tsg::print("GLFW Error {}: {}", error, description);
	tsg::logger::get_instance().write("GLFW Error {}: {}", error, description);
	assert(0);
}

glfw_game::glfw_game() {
	glfwSetErrorCallback(error_callback);
	init();
}

glfw_game::~glfw_game() {
	glfwTerminate();
}

bool glfw_game::init() {
	return GLFW_TRUE == glfwInit();
}

void glfw_game::quit() {
	// TODO: any additional cleanup
}
#endif