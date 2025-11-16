#pragma once

#include "glfw_adaptor.h"
#include "texture.h"
#include "asset.h"
#include "gl_includes.h"

/* tsg includes */
#include <tsg/types.h>

class glfw_texture : public virtual texture, public tsg::adapter_v<GLuint>
{
public:
	glfw_texture(int width = 0, int height = 0);
	virtual ~glfw_texture();
public:
	void set_active() override;
protected:
};

