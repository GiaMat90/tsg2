#pragma once

#include "gl_includes.h"

/* tsg includes */
#include <tsg/types.h>
#include <window.h>

class glfw_window : public window, public tsg::adapter_r<GLFWwindow> {
public:
	glfw_window(const std::string& s = "", const window::width_t w = 0u, const window::height_t h = 0u);
	~glfw_window();	
public:
	height_t get_height();
	width_t get_width();
};

