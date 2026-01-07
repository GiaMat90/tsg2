#pragma once

#include <cursor.h>

#include "glfw_window.h"

class glfw_cursor : public cursor<glfw_window, glfw_cursor> {
public:
	glfw_cursor(glfw_window* const w, const std::string& image = "");
	~glfw_cursor();
public:
	void set_cursor_image(const std::string& image);
private:
	GLFWcursor* m_cursor{ nullptr };
};