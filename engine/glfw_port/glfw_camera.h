#pragma once

#ifdef GLFW_GAME

#include <camera.h>
#include "glfw_window.h"
#include <glm/glm.hpp>

class glfw_renderer;

class glfw_camera : public camera<glfw_window> {
	friend glfw_renderer;
public:
	glfw_camera(glfw_window * const);
	virtual ~glfw_camera();
	void init() override;
	void set_initial_zoom(const float z) override;
private:
	glm::mat4 get_view();
	glm::mat4 get_projection();

};

#endif