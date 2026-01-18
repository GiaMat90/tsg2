#ifdef GLFW_GAME 

#include "glfw_sprite.h"
#include "gl_utility.h" // gl_check_error

/* tsg includes */
#include <tsg/logger.h>
#include <tsg/io.h>         // print

#include <stbi_wrap/stbi.h>


sprite* sprite::create_sprite() {
    return new glfw_sprite();
}

void glfw_sprite::load(const std::string& asset) {
    if (!asset.empty()) {
        stbi img(asset);
        if (img.load()) {
            m_width = img.get_data()->m_width;
			m_height = img.get_data()->m_height;
            GLenum rgba_format;
            switch (img.get_data()->m_channels)
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
			// activate the texture to apply changes
            set_active();
            // Upload image data to GPU
            glTexImage2D(GL_TEXTURE_2D, 0, rgba_format, m_width, m_height, 0, rgba_format, GL_UNSIGNED_BYTE, img.get_data()->m_data);
            check_error(__FILE__, __LINE__);

            // Generate mipmaps
            glGenerateMipmap(GL_TEXTURE_2D);
            check_error(__FILE__, __LINE__);
        }
        else {
            tsg::logger::get_instance().write("Error loading texture {}, reason {}", asset, img.get_failure_reason());
			assert(0); // failed to load texture
        }
    }
}

void glfw_sprite::unload() {
    if (m_raw_attribute) {
        glDeleteTextures(1, &m_raw_attribute);
        m_raw_attribute = 0;
        m_width = 0;
        m_height = 0;
    }
}

#else
static_assert(false, "GLFW_GAME is not defined");
#endif
