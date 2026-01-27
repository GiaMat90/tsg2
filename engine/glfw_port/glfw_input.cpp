#ifdef GLFW_GAME 

#include "input.h"
#include "glfw_input.h"
#include "gl_includes.h"
#include "gl_utility.h" // gl_check_error

/* std inlcudes */
#include <cassert>
#include <map>
#include <unordered_map>

static const std::map<input_key, int> g_keyboard_glfw_mapping{
	{ input_key::key_esc, GLFW_KEY_ESCAPE},
	{ input_key::key_ret, GLFW_KEY_ENTER },
	{ input_key::key_lctrl, GLFW_KEY_LEFT_CONTROL },
	{ input_key::key_rctrl, GLFW_KEY_RIGHT_CONTROL },
	{ input_key::key_lshift, GLFW_KEY_LEFT_SHIFT },
	{ input_key::key_rshift, GLFW_KEY_RIGHT_SHIFT },
	{ input_key::key_lalt, GLFW_KEY_LEFT_ALT },
	{ input_key::key_ralt, GLFW_KEY_RIGHT_ALT },
	{ input_key::key_tab, GLFW_KEY_TAB },
	{ input_key::key_space, GLFW_KEY_SPACE },
	{ input_key::key_left, GLFW_KEY_LEFT },
	{ input_key::key_right, GLFW_KEY_RIGHT },
	{ input_key::key_down, GLFW_KEY_DOWN },
	{ input_key::key_up, GLFW_KEY_UP },
	{ input_key::key_q, GLFW_KEY_Q },
	{ input_key::key_w, GLFW_KEY_W },
	{ input_key::key_e, GLFW_KEY_E },
	{ input_key::key_r, GLFW_KEY_R },
	{ input_key::key_t, GLFW_KEY_T },
	{ input_key::key_y, GLFW_KEY_Y },
	{ input_key::key_u, GLFW_KEY_U },
	{ input_key::key_i, GLFW_KEY_I },
	{ input_key::key_o, GLFW_KEY_O },
	{ input_key::key_p, GLFW_KEY_P },
	{ input_key::key_a, GLFW_KEY_A },
	{ input_key::key_s, GLFW_KEY_S },
	{ input_key::key_d, GLFW_KEY_D },
	{ input_key::key_f, GLFW_KEY_F },
	{ input_key::key_g, GLFW_KEY_G },
	{ input_key::key_h, GLFW_KEY_H },
	{ input_key::key_j, GLFW_KEY_J },
	{ input_key::key_k, GLFW_KEY_K },
	{ input_key::key_l, GLFW_KEY_L },
	{ input_key::key_z, GLFW_KEY_Z },
	{ input_key::key_x, GLFW_KEY_X },
	{ input_key::key_c, GLFW_KEY_C },
	{ input_key::key_v, GLFW_KEY_V },
	{ input_key::key_b, GLFW_KEY_B },
	{ input_key::key_n, GLFW_KEY_N },
	{ input_key::key_m, GLFW_KEY_M },
};

static const std::unordered_map<input_mouse, int> g_mouse_glfw_mapping{
	{ input_mouse::left, GLFW_MOUSE_BUTTON_LEFT },
	{ input_mouse::right, GLFW_MOUSE_BUTTON_RIGHT },
	{ input_mouse::middle, GLFW_MOUSE_BUTTON_MIDDLE },
};
glfw_input::glfw_input(glfw_window * const w, glfw_camera * const c) : input<glfw_window, glfw_camera, glfw_input>(w, c) {}
glfw_input::~glfw_input(){}

// overloaded methods
bool glfw_input::is_key_pressed(const input_key key) {
	return glfwGetKey(m_window->cget_raw_attribute(), g_keyboard_glfw_mapping.at(key)) == GLFW_PRESS;
};
// mouse
bool glfw_input::is_mouse_clicked(const input_mouse side) {
	const bool res{ glfwGetMouseButton(m_window->cget_raw_attribute(), g_mouse_glfw_mapping.at(side)) == GLFW_RELEASE };
	check_error(__FILE__, __LINE__);
	return res;
}
bool glfw_input::is_mouse_pressed(const input_mouse side) {
	const bool res{ glfwGetMouseButton(m_window->cget_raw_attribute(), g_mouse_glfw_mapping.at(side)) == GLFW_PRESS };
	check_error(__FILE__, __LINE__);
	return res;
}
bool glfw_input::is_mouse_released(const input_mouse side) {
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