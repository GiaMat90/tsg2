#pragma once

#ifdef GLFW_GAME

#include "glfw_camera.h"
#include "gl_includes.h"
#include "gl_utility.h"
// tsg includes
#include <tsg/logger.h>
#include <tsg/io.h>
// std includes
#include <cassert>
// glm includes
#include <glm/gtc/matrix_transform.hpp>

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

class glfw_camera_data {
	friend glfw_camera;
	friend void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	friend void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
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
	float m_speed{ 0.0f };
	float m_sensitivity{ 0.0f };
	float m_zoom{ 1.0f };
};

static glfw_camera_data camera_data;

static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
	/* Initial value */
	static float pos_x{ static_cast<float>(xposIn) };
	static float pos_y{ static_cast<float>(yposIn) };
	// 2D case
	camera_data.m_position.x += xposIn - pos_x;
	camera_data.m_position.y -= yposIn - pos_y;
	// compute process movement
	camera_data.m_yaw += xposIn - pos_x;
	camera_data.m_pitch += yposIn - pos_y;
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
	pos_x = xposIn;
	pos_y = yposIn;
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	/* TODO: Evaluate to crop the zoom */
	camera_data.m_zoom += static_cast<float>(yoffset);
}

glfw_camera::glfw_camera(glfw_window * const w) : camera(w) {
#if 1
	glfwSetCursorPosCallback(m_window->get_adaptee_r(), mouse_callback);
	glfwSetScrollCallback(m_window->get_adaptee_r(), scroll_callback);
	// make system sensible to mouse
	glfwSetInputMode(m_window->get_adaptee_r(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//check_error(); // <- get an assert, TODO understand why and fix
#endif
}

glfw_camera::~glfw_camera() {}

void glfw_camera::init() {
	glfwSetCursorPosCallback(m_window->get_adaptee_r(), mouse_callback);
	glfwSetScrollCallback(m_window->get_adaptee_r(), scroll_callback);
	// make system sensible to mouse
	glfwSetInputMode(m_window->get_adaptee_r(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	check_error();
}

glm::mat4 glfw_camera::get_view() {
	/* As per now this is good tested for 2D case. */
	if (m_target) {
		camera_data.m_position.x = m_target->get_position()[0];
		camera_data.m_position.y = m_target->get_position()[1];
		camera_data.m_position.z = m_target->get_position()[2];
		return glm::lookAt(
			glm::vec3(
				camera_data.m_position.x,
				camera_data.m_position.y,
				1.0f),
			glm::vec3(camera_data.m_position.x, camera_data.m_position.y, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f)
		);
	}
	else {
		//return glm::lookAt(camera_data.m_position, camera_data.m_position + camera_data.m_front, camera_data.m_up);
		//return glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		return glm::lookAt(
			glm::vec3(
				camera_data.m_position.x / m_window->get_width(),
				camera_data.m_position.y / m_window->get_height(),
				1.0f /*camera_data.m_zoom * static_cast<float>(m_window->get_width()) / static_cast<float>(m_window->get_height())*/),
			glm::vec3(camera_data.m_position.x / m_window->get_width(), camera_data.m_position.y / m_window->get_height(), 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f)
		);
	}
	//return glm::mat4(1.0f);
}

glm::mat4 glfw_camera::get_projection() {
	// TODO: understand well the role of the last two parameters: its correct that they are in camera class?
	return glm::perspective(glm::radians(camera_data.m_zoom), static_cast<float>(m_window->get_width()) / static_cast<float>(m_window->get_height()), 0.1f, 100.0f);
}

#endif