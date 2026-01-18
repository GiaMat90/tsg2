#ifdef GLFW_GAME 

#define GLAD_GL_IMPLEMENTATION
#include "gl_includes.h"

#else
static_assert(false, "GLFW_GAME is not defined");
#endif
