#pragma once

#include "input.h"
#include "glfw_window.h"

class glfw_camera;

class glfw_input : public input<glfw_window, glfw_camera, glfw_input>{
public:
	glfw_input(glfw_window * const w, glfw_camera * const c);
	~glfw_input();
public:
	// overloaded methods
	bool is_key_pressed(const INPUT_KEY key) override final;
	// mouse
	bool is_mouse_clicked(const INPUT_MOUSE side) override final;
	bool is_mouse_pressed(const INPUT_MOUSE side) override final;
	bool is_mouse_released(const INPUT_MOUSE side) override final;
	void get_mouse_position(float& x, float& y) override final;
};