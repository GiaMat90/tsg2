#pragma once

#include "gl_includes.h"

/* tsg includes */
#include <tsg/types.h>

/* std includes */
#include <filesystem>

class shader : public tsg::adapter_v<GLuint>{
public:
	shader() = default;
	shader(const char* vertex_path, const char* fragment_path);
	~shader();
	void init(const char* vertex_path, const char* fragment_path);
	void use();
	void unuse();
private:
	enum class SHADER_TYPE : uint32_t {
		VERTEX	 = 0,
		FRAGMENT = 1
	};
	GLuint load(const char*, const SHADER_TYPE type);
private:
};