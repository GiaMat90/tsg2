#pragma once

#ifdef GLFW_GAME

#include "glfw_camera.h"
#include "gl_includes.h"
// tsg includes
#include <tsg/logger.h>
#include <tsg/io.h>
// std includes
#include <cassert>

static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
	/* do something */
	//assert(0);
	tsg::logger::get_instance().write("Mouse callback (x,y)=({},{})", xposIn, yposIn);
	tsg::print("Mouse callback (x,y)=({},{})", xposIn, yposIn);
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	/* TODO */
	//assert(0);
	tsg::logger::get_instance().write("Scroll callback (x,y)=({},{})", xoffset, yoffset);
	tsg::print("Scroll callback (x,y)=({},{})", xoffset, yoffset);
}

glfw_camera::glfw_camera(glfw_window * const w) : camera(w) {
	glfwSetCursorPosCallback(m_window->get_adaptee_r(), mouse_callback);
	glfwSetScrollCallback(m_window->get_adaptee_r(), scroll_callback);
	// make system sensible to mouse
	glfwSetInputMode(m_window->get_adaptee_r(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

glfw_camera::~glfw_camera() {}

#endif