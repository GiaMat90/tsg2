#ifdef GLFW_GAME 

#include "input.h"
#include "glfw_input.h"
#include "gl_includes.h"
#include "gl_utility.h" // gl_check_error

/* std inlcudes */
#include <cassert>
#include <map>
#include <unordered_map>

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

static const std::unordered_map<INPUT_MOUSE, int> g_mouse_glfw_mapping{
	{ INPUT_MOUSE::LEFT, GLFW_MOUSE_BUTTON_LEFT },
	{ INPUT_MOUSE::RIGHT, GLFW_MOUSE_BUTTON_RIGHT },
	{ INPUT_MOUSE::MIDDLE, GLFW_MOUSE_BUTTON_MIDDLE },
};
glfw_input::glfw_input(glfw_window * const w, glfw_camera * const c) : input<glfw_window, glfw_camera, glfw_input>(w, c) {}
glfw_input::~glfw_input(){}

// overloaded methods
bool glfw_input::is_key_pressed(const INPUT_KEY key) {
	return glfwGetKey(m_window->cget_raw_attribute(), g_keyboard_glfw_mapping.at(key)) == GLFW_PRESS;
};
// mouse
bool glfw_input::is_mouse_clicked(const INPUT_MOUSE side) {
	const bool res{ glfwGetMouseButton(m_window->cget_raw_attribute(), g_mouse_glfw_mapping.at(side)) == GLFW_RELEASE };
	check_error(__FILE__, __LINE__);
	return res;
}
bool glfw_input::is_mouse_pressed(const INPUT_MOUSE side) {
	const bool res{ glfwGetMouseButton(m_window->cget_raw_attribute(), g_mouse_glfw_mapping.at(side)) == GLFW_PRESS };
	check_error(__FILE__, __LINE__);
	return res;
}
bool glfw_input::is_mouse_released(const INPUT_MOUSE side) {
	const bool res{ glfwGetMouseButton(m_window->cget_raw_attribute(), g_mouse_glfw_mapping.at(side)) == GLFW_RELEASE };
	check_error(__FILE__, __LINE__);
	return res;
}
void glfw_input::get_mouse_position(float& x, float& y) {
	double xpos, ypos;
	glfwGetCursorPos(m_window->cget_raw_attribute(), &xpos, &ypos);
	x = static_cast<float>(xpos) / static_cast<float>(m_window->get_width());
	y = static_cast<float>(ypos) / static_cast<float>(m_window->get_height());
}

#else
static_assert(false, "GLFW_GAME is not defined");
#endif