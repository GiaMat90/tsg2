#ifdef GLFW_GAME 

#include "glfw_texture.h"
#include "gl_utility.h" // gl_check_error

/* tsg includes */
#include <tsg/logger.h>
#include <tsg/io.h>         // print

texture* texture::create_texture() {
	return new glfw_texture();
}

glfw_texture::glfw_texture(int w, int h) : texture(w, h) {
    // Generate texture
    glGenTextures(1, &m_adaptee);
    gl_check_error(__FILE__, __LINE__);

    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, m_adaptee);
    gl_check_error(__FILE__, __LINE__);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    gl_check_error(__FILE__, __LINE__);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    gl_check_error(__FILE__, __LINE__);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gl_check_error(__FILE__, __LINE__);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gl_check_error(__FILE__, __LINE__);
}
glfw_texture::~glfw_texture() {
    if (m_adaptee) {
        glDeleteTextures(1, &m_adaptee);
        m_adaptee = 0u;
    }
}

void glfw_texture::set_active() {
    glBindTexture(GL_TEXTURE_2D, get_adaptee_v());
    gl_check_error(__FILE__, __LINE__);
}

#endif