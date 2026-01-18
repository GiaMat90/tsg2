#pragma once

#ifdef GLFW_GAME

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#else
static_assert(false, "GLFW_GAME is not defined");
#endif
