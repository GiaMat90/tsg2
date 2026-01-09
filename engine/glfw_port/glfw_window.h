#pragma once

#include "gl_includes.h"
// engine includes
#include "glfw_adapter.h"
#include <window.h>

class glfw_window : public window, public glfw_adapter<glfw_window, GLFWwindow*> {
public:
	glfw_window(const std::string& s = "", const window::width_t w = 0u, const window::height_t h = 0u);
	~glfw_window();	
public:
	height_t get_height();
	width_t get_width();
};

