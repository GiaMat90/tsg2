#ifdef GLFW_GAME 

#include "input.h"
#include "glfw_input.h"
#include "gl_includes.h"
#include "gl_utility.h" // gl_check_error

/* std inlcudes */
#include <cassert>
#include <map>

static const std::map<INPUT_KEY, int> g_keyboard_glfw_mapping{
	{ INPUT_KEY::KEY_ESC, GLFW_KEY_ESCAPE},
	{ INPUT_KEY::KEY_RET, GLFW_KEY_ENTER },
	{ INPUT_KEY::KEY_LCTRL, GLFW_KEY_LEFT_CONTROL },
	{ INPUT_KEY::KEY_RCTRL, GLFW_KEY_RIGHT_CONTROL },
	{ INPUT_KEY::KEY_LSHIFT, GLFW_KEY_LEFT_SHIFT },
	{ INPUT_KEY::KEY_RSHIFT, GLFW_KEY_RIGHT_SHIFT },
	{ INPUT_KEY::KEY_LALT, GLFW_KEY_LEFT_ALT },
	{ INPUT_KEY::KEY_RALT, GLFW_KEY_RIGHT_ALT },
	{ INPUT_KEY::KEY_TAB, GLFW_KEY_TAB },
	{ INPUT_KEY::KEY_SPACE, GLFW_KEY_SPACE },
	{ INPUT_KEY::KEY_LEFT, GLFW_KEY_LEFT },
	{ INPUT_KEY::KEY_RIGHT, GLFW_KEY_RIGHT },
	{ INPUT_KEY::KEY_DOWN, GLFW_KEY_DOWN },
	{ INPUT_KEY::KEY_UP, GLFW_KEY_UP },
	{ INPUT_KEY::KEY_Q, GLFW_KEY_Q },
	{ INPUT_KEY::KEY_W, GLFW_KEY_W },
	{ INPUT_KEY::KEY_E, GLFW_KEY_E },
	{ INPUT_KEY::KEY_R, GLFW_KEY_R },
	{ INPUT_KEY::KEY_T, GLFW_KEY_T },
	{ INPUT_KEY::KEY_Y, GLFW_KEY_Y },
	{ INPUT_KEY::KEY_U, GLFW_KEY_U },
	{ INPUT_KEY::KEY_I, GLFW_KEY_I },
	{ INPUT_KEY::KEY_O, GLFW_KEY_O },
	{ INPUT_KEY::KEY_P, GLFW_KEY_P },
	{ INPUT_KEY::KEY_A, GLFW_KEY_A },
	{ INPUT_KEY::KEY_S, GLFW_KEY_S },
	{ INPUT_KEY::KEY_D, GLFW_KEY_D },
	{ INPUT_KEY::KEY_F, GLFW_KEY_F },
	{ INPUT_KEY::KEY_G, GLFW_KEY_G },
	{ INPUT_KEY::KEY_H, GLFW_KEY_H },
	{ INPUT_KEY::KEY_J, GLFW_KEY_J },
	{ INPUT_KEY::KEY_K, GLFW_KEY_K },
	{ INPUT_KEY::KEY_L, GLFW_KEY_L },
	{ INPUT_KEY::KEY_Z, GLFW_KEY_Z },
	{ INPUT_KEY::KEY_X, GLFW_KEY_X },
	{ INPUT_KEY::KEY_C, GLFW_KEY_C },
	{ INPUT_KEY::KEY_V, GLFW_KEY_V },
	{ INPUT_KEY::KEY_B, GLFW_KEY_B },
	{ INPUT_KEY::KEY_N, GLFW_KEY_N },
	{ INPUT_KEY::KEY_M, GLFW_KEY_M },
};

glfw_input::glfw_input(glfw_window * w) : input<glfw_window, glfw_input>(w) {}
glfw_input::~glfw_input(){}

// overloaded methods
bool glfw_input::is_key_pressed(const INPUT_KEY key) {
	const bool res{ glfwGetKey(m_window->get_adaptee_r(),g_keyboard_glfw_mapping.at(key)) == GLFW_PRESS };
	check_error();
	return res;
};
// mouse
bool glfw_input::is_mouse_clicked(const INPUT_MOUSE side) {
	/* TODO */
	assert(false);
	return false;
}
bool glfw_input::is_mouse_pressed(const INPUT_MOUSE side) {
	/* TODO */
	assert(false);
	return false;
}
bool glfw_input::is_mouse_released(const INPUT_MOUSE side) {
	/* TODO */
	assert(false);
	return false;
}

#endif