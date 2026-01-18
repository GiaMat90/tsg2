#if GLFW_GAME

#include "glfw_event.h"
#include "glfw_window.h"
#include "gl_utility.h"
#include "gl_includes.h"


glfw_event::glfw_event(glfw_window* const w, glfw_input* const i) : game_event<glfw_window, glfw_input, glfw_event>(w, i) {};

GAME_EVENTS glfw_event::get_events() {
	glfwPollEvents();
	if(m_input->is_key_pressed(m_event_key_map[GAME_EVENTS::QUIT])) {
		return GAME_EVENTS::QUIT;
	}
	check_error(__FILE__, __LINE__);
}

#else
static_assert(false, "GLFW_GAME is not defined");
#endif