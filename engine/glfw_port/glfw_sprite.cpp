#ifdef GLFW_GAME 

#include "glfw_sprite.h"
#include "gl_utility.h" // gl_check_error

/* tsg includes */
#include <tsg/logger.h>
#include <tsg/io.h>         // print

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>      // stbi_load

sprite* sprite::create_sprite() {
    return new glfw_sprite();
}

void glfw_sprite::load(const std::string& asset) {
    if (!asset.empty()) {
        set_active();
        int nrChannels;
        unsigned char* data = stbi_load(asset.c_str(), &m_width, &m_height, &nrChannels, 0);
        if (data) {
            GLenum rgba_format;
            switch (nrChannels)
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
           
            // Upload image data to GPU
            glTexImage2D(GL_TEXTURE_2D, 0, rgba_format, m_width, m_height, 0, rgba_format, GL_UNSIGNED_BYTE, data);
            check_error(__FILE__, __LINE__);

            // Generate mipmaps
            glGenerateMipmap(GL_TEXTURE_2D);
            check_error(__FILE__, __LINE__);
        }
        else {
            tsg::logger::get_instance().write("Error loading texture {}, reason {}", asset, stbi_failure_reason());
			assert(0); // failed to load texture
        }
        stbi_image_free(data);
    }
}

void glfw_sprite::unload() {
    if (m_adaptee) {
        glDeleteTextures(1, &m_adaptee);
        m_adaptee = 0;
        m_width = 0;
        m_height = 0;
    }
}

#endif
