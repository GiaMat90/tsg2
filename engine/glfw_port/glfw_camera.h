#pragma once

#ifdef GLFW_GAME

#include <camera.h>
#include "glfw_window.h"

class glfw_camera : public camera<glfw_window> {
public:
	glfw_camera(glfw_window * const);
	virtual ~glfw_camera();
private:

};

#endif