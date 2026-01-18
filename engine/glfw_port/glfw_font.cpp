#ifdef GLFW_GAME 

#include "glfw_font.h"

font* font::create_font() {
	return new glfw_font();
}

glfw_font::glfw_font() : font() {}

glfw_font::glfw_font(const std::string& file_name, const color& c, const text_position& p, const std::size_t dim) : font(file_name, c, p) {
	load_font(file_name, dim);
}

glfw_font::~glfw_font() {}

void glfw_font::load_font(const std::string& file_name, const std::size_t dim) {
	/* TODO */
	assert(0);
}

#else
static_assert(false, "GLFW_GAME is not defined");
#endif