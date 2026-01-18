#ifdef GLFW_GAME 

#include "glfw_window.h"
#include "gl_includes.h"
#include "gl_utility.h"

/* tsg includes */
#include <tsg/logger.h>
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
	m_raw_attribute = glfwCreateWindow(w, h, t.c_str(), nullptr, nullptr);

	if (!m_raw_attribute)
	{
		const char* description;
		int code = glfwGetError(&description);
		tsg::print("Error {}:{}", code, description);
		tsg::logger::get_instance().write("Error {}:{}", code, description);
		assert(0);
	}
	glfwMakeContextCurrent(m_raw_attribute);
	glfwSetFramebufferSizeCallback(m_raw_attribute, resize_window_callback);
}

glfw_window::~glfw_window() { /* Nothing To Do */ }

window::height_t glfw_window::get_height() {
	update();
	return m_height;
};
window::width_t glfw_window::get_width() {
	update();
	return m_width;
};

void glfw_window::update() {
	glfwGetFramebufferSize(m_raw_attribute, &m_width, &m_height);
};

#else
static_assert(false, "GLFW_GAME is not defined");
#endif


