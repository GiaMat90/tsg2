#include "glfw_cursor.h"
#include <stbi_wrap/stbi.h>

glfw_cursor::glfw_cursor(glfw_window* const w, const std::string& image) : cursor<glfw_window, glfw_cursor>(w, image) {
    set_cursor_image(image);
}

glfw_cursor::~glfw_cursor() {
	//TODO: implement cursor destruction with GLFW
	if(m_cursor) {
		glfwDestroyCursor(m_cursor);
		m_cursor = nullptr;
	}
}

void glfw_cursor::set_cursor_image(const std::string& image) {
    if (!image.empty()) {
		stbi stbi_img(image);
        if (stbi_img.load()) {
            GLenum rgba_format;
            switch (stbi_img.get_data()->m_channels)
            {
            case 1:
                rgba_format = GL_RED;
                break;
            case 3:
                rgba_format = GL_RGB;
                break;
            case 4:
                rgba_format = GL_RGBA;
                break;
            default:
                rgba_format = GL_RGB;
                break;
            }
            GLFWimage glfw_img;
            glfw_img.width = 16;
            glfw_img.height = 16;
            glfw_img.pixels = stbi_img.get_data()->m_data;
            m_cursor = glfwCreateCursor(&glfw_img, 0, 0);
            assert(m_cursor);
        }
    }
}