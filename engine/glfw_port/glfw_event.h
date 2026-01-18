#pragma once

#if GLFW_GAME

#include "game_event.h"
#include "glfw_window.h"
#include "glfw_input.h"

class glfw_event : public game_event<glfw_window, glfw_input, glfw_event>{
public:
	glfw_event(glfw_window * const w, glfw_input * const i);
	~glfw_event() = default;
public:
	GAME_EVENTS get_events();
};

#else
static_assert(false, "GLFW_GAME is not defined");
#endif