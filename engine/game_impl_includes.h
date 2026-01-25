#pragma once

/* Graphic impl includes */
#ifdef GLFW_GAME
#include "glfw_port/glfw_port.h"
#elifdef VULKAN_GAME
/* TODO */
#include "vulkan_port/vulkan_port.h"
#elifdef GLES_GAME
/* TODO */
#include "gles_port/gles_port.h"
#elifdef DIRECTX
/* TODO */
#endif
