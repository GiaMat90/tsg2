#include "glfw_cursor.h"
#include <stbi_wrap/stbi.h>

static cursor<glfw_window,glfw_cursor>::event g_cursor_event = cursor<glfw_window, glfw_cursor>::event::none;

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		g_cursor_event = cursor<glfw_window, glfw_cursor>::event::right_click;
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        g_cursor_event = cursor<glfw_window, glfw_cursor>::event::left_click;
    } else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
        g_cursor_event = cursor<glfw_window, glfw_cursor>::event::middle_click;
    } else {
		g_cursor_event = cursor<glfw_window, glfw_cursor>::event::none;
    }
}
glfw_cursor::glfw_cursor(glfw_window* const w, const std::string& image) : cursor<glfw_window, glfw_cursor>(w, image) {
    set_cursor_image(image);
    glfwSetMouseButtonCallback(m_window->get_adaptee_r(), mouse_button_callback);
}

glfw_cursor::~glfw_cursor() {
	//TODO: implement cursor destruction with GLFW
	if(m_cursor) {
		glfwDestroyCursor(m_cursor);
		m_cursor = nullptr;
	}
}

void glfw_cursor::set_cursor_image(const std::string& image) {
    if (false == image.empty()) {
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
            glfw_img.width = stbi_img.get_data()->m_width;
            glfw_img.height = stbi_img.get_data()->m_height;
            glfw_img.pixels = stbi_img.get_data()->m_data;
            m_cursor = glfwCreateCursor(&glfw_img, 0, 0);
            assert(m_cursor);
            glfwSetCursor(m_window->get_adaptee_r(), m_cursor);
        }
    }
    else {
        glfwSetCursor(m_window->get_adaptee_r(), NULL);
    }
}

tsg::vector<float, 2> glfw_cursor::get_position() {
    double xpos, ypos;
    glfwGetCursorPos(m_window->get_adaptee_r(), &xpos, &ypos);
    xpos /= static_cast<float>(m_window->get_width());
    ypos /= static_cast<float>(m_window->get_height());
    return tsg::vector<float, 2>({ static_cast<float>(xpos), static_cast<float>(ypos) });
}

cursor<glfw_window, glfw_cursor>::event glfw_cursor::get_event() {
    return g_cursor_event;
}