#pragma once

#ifdef GLFW_GAME

#include <camera.h>
#include "glfw_window.h"
#include <glm/glm.hpp>

class glfw_renderer;
class glfw_cursor;

class glfw_camera : public camera<glfw_window, glfw_camera> {
	friend glfw_renderer;
	friend glfw_cursor;
public:
	glfw_camera(glfw_window * const);
	virtual ~glfw_camera();
	void init() const;
	void update_camera(input_engine* const) const;
	void set_initial_zoom(const float z) const;
private:
	glm::mat4 get_view() const;
	glm::mat4 get_projection() const;

};

#endif