#pragma once

#include <cursor.h>
// engine includes
#include "glfw_window.h"
#include "glfw_camera.h"
// tsg includes
#include <tsg/math.h>

class glfw_cursor : public cursor<glfw_window, glfw_camera, glfw_cursor> {
public:
	glfw_cursor(glfw_window* const w, glfw_camera* const c, const std::string& image = "");
	~glfw_cursor();
public:
	void set_cursor_image(const std::string& image);
	template <std::size_t Dim> requires (Dim == 2 || Dim == 3)
	inline tsg::vector<float, Dim> get_world_position() const {
		double xpos, ypos;
		glfwGetCursorPos(m_window->cget_raw_attribute(), &xpos, &ypos);
		return m_camera->screen_to_world<Dim>(static_cast<float>(xpos), static_cast<float>(ypos));
	};
	screen_position get_screen_position() const;
	event get_event() const;
private:
	GLFWcursor* m_cursor{ nullptr };
};