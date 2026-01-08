#pragma once

#include <cursor.h>
// engine includes
#include "glfw_window.h"
// tsg includes
#include <tsg/math.h>

class glfw_cursor : public cursor<glfw_window, glfw_cursor> {
public:
	glfw_cursor(glfw_window* const w, const std::string& image = "");
	~glfw_cursor();
public:
	void set_cursor_image(const std::string& image);
	position get_position() const;
	event get_event() const;
private:
	GLFWcursor* m_cursor{ nullptr };
};