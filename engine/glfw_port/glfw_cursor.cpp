#include "glfw_cursor.h"
// engine includes
#include <stbi_wrap/stbi.h>
// tsg includes
#include <tsg/logger.h>
// glm includes
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

static cursor<glfw_window, glfw_camera, glfw_cursor>::event g_cursor_event = cursor<glfw_window, glfw_camera, glfw_cursor>::event::none;

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		g_cursor_event = cursor<glfw_window, glfw_camera, glfw_cursor>::event::right_click;
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        g_cursor_event = cursor<glfw_window, glfw_camera, glfw_cursor>::event::left_click;
    }
    else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
        g_cursor_event = cursor<glfw_window, glfw_camera, glfw_cursor>::event::middle_click;
    }
    else {
        g_cursor_event = cursor<glfw_window, glfw_camera, glfw_cursor>::event::none;
    }
}

glfw_cursor::glfw_cursor(glfw_window* const w, glfw_camera* const c, const std::string& image) : cursor<glfw_window, glfw_camera, glfw_cursor>(w, c, image) {
    set_cursor_image(image);
    glfwSetMouseButtonCallback(m_window->cget_raw_attribute(), mouse_button_callback);
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
            glfwSetCursor(m_window->cget_raw_attribute(), m_cursor);
        }
    }
    else {
        glfwSetCursor(m_window->cget_raw_attribute(), NULL);
    }
}

tsg::vector<float, 2> glfw_cursor::get_position() const {
    double xpos, ypos;
    glfwGetCursorPos(m_window->cget_raw_attribute(), &xpos, &ypos);
	// Reverse the viewport transformation: from screen coordinates to NDC
    float x = (2.0f * xpos) / m_window->get_width() - 1.0f;  
    float y = 1.0f - (2.0f * ypos) / m_window->get_height();  
	auto model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	model = glm::translate(model, glm::vec3(x, y, 0.0f));
    auto world_position = glm::inverse(m_camera->get_projection() * m_camera->get_view()) * glm::vec4(x, y, 0.0f, 1.0f);
	return tsg::vector<float, 2>({ world_position.x, world_position.y });
}

cursor<glfw_window, glfw_camera, glfw_cursor>::event glfw_cursor::get_event() const {
    return g_cursor_event;
}