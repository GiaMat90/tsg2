#pragma once

#include "glfw_adapter.h"
#include "texture.h"
#include "asset.h"
#include "gl_includes.h"

class glfw_texture : public virtual texture, public glfw_adapter<GLuint>
{
public:
	glfw_texture(int width = 0, int height = 0);
	virtual ~glfw_texture();
public:
	void set_active() const override;
protected:
};

