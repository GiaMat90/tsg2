#pragma once

#ifdef GLFW_GAME

#include "glfw_camera.h"
#include "gl_includes.h"
#include "gl_utility.h"
#include "input.h"
// tsg includes
#include <tsg/logger.h>
#include <tsg/io.h>
// std includes
#include <cassert>
// glm includes
#include <glm/gtc/matrix_transform.hpp>

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
static void mouse_callback_2D(GLFWwindow* window, double xposIn, double yposIn);
static void mouse_callback_3D(GLFWwindow* window, double xposIn, double yposIn);

class glfw_camera_data {
	friend camera<glfw_window, glfw_camera>;
	friend glfw_camera;
	friend void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	friend void mouse_callback_2D(GLFWwindow* window, double xposIn, double yposIn);
	friend void mouse_callback_3D(GLFWwindow* window, double xposIn, double yposIn);
	friend void mouse_2D();
private:
	// positions
	glm::vec3 m_position{};
	glm::vec3 m_front{};
	glm::vec3 m_up{};
	glm::vec3 m_right{};
	glm::vec3 m_world_up{};
	// euler angles
	float m_yaw{ 0.0f };
	float m_pitch{ 0.0f };
	// parameters
	float m_speed{ 10.0f };
	float m_sensitivity{ 0.0f };
	float m_zoom{ 0.0f };
};

struct mouse_position {
	float x{ 0.0f };
	float y{ 0.0f };
};

static glfw_camera_data camera_data;
static mouse_position last_mouse_pos;

/* OpenGL callbacks */
static void capture_mouse_position(GLFWwindow*, double xposIn, double yposIn) {
	last_mouse_pos.x = static_cast<float>(xposIn);
	last_mouse_pos.y = static_cast<float>(yposIn);
}
static void empty_mouse_callback(GLFWwindow*, double, double) {
	return; // do nothing
};
static void mouse_callback_2D(GLFWwindow*, double xposIn, double yposIn) {
	/* Initial value */
	static float pos_x{ static_cast<float>(xposIn) };
	static float pos_y{ static_cast<float>(yposIn) };
	// 2D case
	camera_data.m_position.x += static_cast<float>(xposIn) - pos_x;
	camera_data.m_position.y -= static_cast<float>(yposIn) - pos_y;
	// update static variables
	pos_x = static_cast<float>(xposIn);
	pos_y = static_cast<float>(yposIn);
};
static void mouse_callback_3D(GLFWwindow*, double xposIn, double yposIn) {
	/* Initial value */
	static float pos_x{ static_cast<float>(xposIn) };
	static float pos_y{ static_cast<float>(yposIn) };
	// compute process movement
	camera_data.m_yaw += static_cast<float>(xposIn) - pos_x;
	camera_data.m_pitch += static_cast<float>(yposIn) - pos_y;
	// computing the new front vector
	camera_data.m_front.x = cosf(glm::radians(camera_data.m_yaw)) * cosf(glm::radians(camera_data.m_pitch));
	camera_data.m_front.y = sinf(glm::radians(camera_data.m_pitch));
	camera_data.m_front.z = sinf(glm::radians(camera_data.m_yaw)) * cosf(glm::radians(camera_data.m_pitch));
	camera_data.m_front = glm::normalize(camera_data.m_front);
	// computing the new right vector
	camera_data.m_right = glm::normalize(glm::cross(camera_data.m_front, camera_data.m_world_up));
	// computing the new up vector
	camera_data.m_up = glm::normalize(glm::cross(camera_data.m_right, camera_data.m_front));
	// update static variables
	pos_x = static_cast<float>(xposIn);
	pos_y = static_cast<float>(yposIn);
}
static void scroll_callback(GLFWwindow*, double, double yoffset)
{
	/* TODO: Evaluate to crop the zoom */
	if ((camera_data.m_zoom + static_cast<float>(yoffset) >= -180.0f) && 
		(camera_data.m_zoom + static_cast<float>(yoffset) < 0.0f)) 
	{
		camera_data.m_zoom += static_cast<float>(yoffset);
	}
}
/* Compute camera methods */
static void mouse_2D() {	
	/* Initial value */
	static float pos_x{ static_cast<float>(last_mouse_pos.x) };
	static float pos_y{ static_cast<float>(last_mouse_pos.y) };
	// 2D case
	camera_data.m_position.x += static_cast<float>(last_mouse_pos.x) - pos_x;
	camera_data.m_position.y -= static_cast<float>(last_mouse_pos.y) - pos_y;
	// update static variables
	pos_x = static_cast<float>(last_mouse_pos.x);
	pos_y = static_cast<float>(last_mouse_pos.y);
}
/* Camera static methods */
void camera<glfw_window, glfw_camera>::translate(const tsg::vector<float, 3>& v) {
	camera_data.m_position.x += v[0];
	camera_data.m_position.y += v[1];
	camera_data.m_position.z += v[2];
}
/* camera class method definitions */
glfw_camera::glfw_camera(glfw_window * const w) : camera(w) {}

glfw_camera::~glfw_camera() {}

void glfw_camera::init() const {
	glfwSetInputMode(m_window->get_adaptee_r(), GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
	if (m_options.test(static_cast<std::size_t>(CAMERA_OPTIONS::MOUSE_CONTROLLED))) {
		// make sure system sensible to mouse
		glfwSetInputMode(m_window->get_adaptee_r(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		if (m_options.test(static_cast<std::size_t>(CAMERA_OPTIONS::SCROLLABLE_ZOOM))) {
			glfwSetScrollCallback(m_window->get_adaptee_r(), scroll_callback);
		}
		if (m_options.test(static_cast<std::size_t>(CAMERA_OPTIONS::LEFT_CLICK_DRAG))) {
			glfwSetCursorPosCallback(m_window->get_adaptee_r(), capture_mouse_position);
		}
		if(m_options.test(static_cast<std::size_t>(CAMERA_OPTIONS::FOLLOW_TARGET))) {
			assert(m_target);
		}
		/* Click events */
	}
	if (m_options.test(static_cast<std::size_t>(CAMERA_OPTIONS::KEY_CONTROLLED))) {
		/* Nothing To Do */
	}		
	/* WIP */
	if (m_options.test(static_cast<std::size_t>(CAMERA_OPTIONS::SCROLLABLE_ZOOM))) {
		glfwSetScrollCallback(m_window->get_adaptee_r(), scroll_callback);
	}
}

void glfw_camera::update_camera(input_engine* const input) const {
	if(m_options.test(static_cast<std::size_t>(CAMERA_OPTIONS::FOLLOW_TARGET))) {
		return;
	}
	if (m_options.test(static_cast<std::size_t>(CAMERA_OPTIONS::LEFT_CLICK_DRAG))) {
		if (input->is_mouse_pressed(INPUT_MOUSE::LEFT))
		{
			mouse_2D();
		}
		if(input->is_mouse_released(INPUT_MOUSE::LEFT))
		{
		}
	} else if (m_options.test(static_cast<std::size_t>(CAMERA_OPTIONS::WASD_CONTROLLED))) {
		if(input->is_key_pressed(INPUT_KEY::KEY_W)) {
			camera_data.m_position.y += camera_data.m_speed;
		} else if (input->is_key_pressed(INPUT_KEY::KEY_S)) {
			camera_data.m_position.y -= camera_data.m_speed;
		} else if ( input->is_key_pressed(INPUT_KEY::KEY_A)) {
			camera_data.m_position.x -= camera_data.m_speed;
		} else if (input->is_key_pressed(INPUT_KEY::KEY_D)) {
			camera_data.m_position.x += camera_data.m_speed;
		} 
	}
}
void glfw_camera::set_initial_zoom(const float z) const {
	assert(z > 0.0f && z < 1.0f);
	camera_data.m_zoom = -180.0f + (z * 180.0f);
}

glm::mat4 glfw_camera::get_view() const {
	/* As per now this is well tested for 2D case. */
	if (m_target) {
		/* target should be already in NDC */
		assert(m_target->get_target_position()[0] * m_target->get_target_position()[1] * m_target->get_target_position()[2] <= 1.0f);
		camera_data.m_position.x = m_target->get_target_position()[0];
		camera_data.m_position.y = m_target->get_target_position()[1];
		camera_data.m_position.z = m_target->get_target_position()[2];
		return glm::lookAt(
			glm::vec3(camera_data.m_position.x, camera_data.m_position.y, 1.0f),
			glm::vec3(camera_data.m_position.x, camera_data.m_position.y, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f)
		);
	}
	else {
		//return glm::lookAt(camera_data.m_position, camera_data.m_position + camera_data.m_front, camera_data.m_up); // <- 3D case
		return glm::lookAt(
			glm::vec3(camera_data.m_position.x / static_cast<float>(m_window->get_width()), camera_data.m_position.y / static_cast<float>(m_window->get_height()), 1.0f /*camera_data.m_zoom * static_cast<float>(m_window->get_width()) / static_cast<float>(m_window->get_height())*/),
			glm::vec3(camera_data.m_position.x / static_cast<float>(m_window->get_width()), camera_data.m_position.y / static_cast<float>(m_window->get_height()), 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f)
		);
	}
}

glm::mat4 glfw_camera::get_projection() const {
	// TODO: understand well the role of the last two parameters: its correct that they are in camera class?
	return glm::perspective(glm::radians(camera_data.m_zoom), static_cast<float>(m_window->get_width()) / static_cast<float>(m_window->get_height()), 0.1f, 100.0f);
}

#endif