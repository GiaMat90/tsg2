#if GLFW_GAME

#include "glfw_event.h"
#include "glfw_window.h"
#include "gl_utility.h"
#include "gl_includes.h"

GAME_EVENTS glfw_event::get_events() {
	glfwPollEvents();
	if (glfwWindowShouldClose(m_window->get_adaptee_r())) {
		//glfwSetWindowShouldClose(w->get_adaptee(), GL_TRUE);
		return GAME_EVENTS::QUIT;
	}
	gl_check_error(__FILE__, __LINE__);
}

#endif