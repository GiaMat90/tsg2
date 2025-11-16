#ifdef GLFW_GAME 

#include "glfw_window.h"
#include "gl_includes.h"
#include "gl_utility.h"

/* tsg includes */
#include <tsg/logger.h>
#include <tsg/types.h>
#include <tsg/io.h>

/* std includes */
#include <cassert>

static void resize_window_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	tsg::logger::get_instance().write("w: {} h: {}", width, height);
}

glfw_window::glfw_window(const std::string& t, const width_t w, const height_t h) : window(t, w, h) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Create an GLFW Window
	m_adaptee = glfwCreateWindow(w, h, t.c_str(), nullptr, nullptr);

	if (!m_adaptee)
	{
		const char* description;
		int code = glfwGetError(&description);
		tsg::print("Error {}:{}", code, description);
		tsg::logger::get_instance().write("Error {}:{}", code, description);
		assert(0);
	}
	glfwMakeContextCurrent(m_adaptee);
	glfwSetFramebufferSizeCallback(m_adaptee, resize_window_callback);
}

glfw_window::~glfw_window() {
	// In openGL it is handled with context that are free in glfwTerminate
	// or maybe I should?
	glfwDestroyWindow(m_adaptee);
}

#endif


