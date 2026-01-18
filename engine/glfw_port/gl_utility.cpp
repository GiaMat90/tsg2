#ifdef GLFW_GAME 

#include "gl_utility.h"
#include "gl_includes.h"

/* tsg includes */
#include <tsg/io.h>		// print
#include <tsg/logger.h> // logger

/* std includes */
#include <cassert>

#ifdef _DEBUG
void check_error(const std::string& f, int r) {
	// Check GLFW errors
	const char* description;
	int code = glfwGetError(&description);
	if (code > GLFW_NO_ERROR) {
		tsg::print("GLFW Error {}:{} throwed in file {} row {}", code, description, f, r);
		tsg::logger::get_instance().write("GLFW Error {}:{} throwed in file {} row {}", code, description, f, r);
		assert(0);
	}
	// Check OpenGL errors
	if (auto err = glGetError()) {
		tsg::print("OpenGL Error {} throwed in file {} row {}", err, f, r);
		tsg::logger::get_instance().write("OpenGL Error {} throwed in file {} row {}", err, f, r);
		assert(0);
	}
};
#else
void gl_check_error(const std::string& f, int r) { return; };
#endif

#else
static_assert(false, "GLFW_GAME is not defined");
#endif