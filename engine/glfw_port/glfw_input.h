#pragma once

#include "input.h"
#include "glfw_window.h"

class glfw_input : public input<glfw_window, glfw_input>{
public:
	glfw_input(glfw_window * w);
	~glfw_input();
public:
	// overloaded methods
	bool is_key_pressed(const INPUT_KEY key) override;
	// mouse
	bool is_mouse_clicked(const INPUT_MOUSE side) override;
	bool is_mouse_pressed(const INPUT_MOUSE side) override;
	bool is_mouse_released(const INPUT_MOUSE side) override;
};