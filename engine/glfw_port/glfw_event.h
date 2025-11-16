#pragma once

#if GLFW_GAME

#include "game_event.h"
#include "glfw_window.h"

class glfw_event : public game_event<glfw_window, glfw_event>{
public:
	GAME_EVENTS get_events();
};

#endif