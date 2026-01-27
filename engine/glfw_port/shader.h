#pragma once

#include "gl_includes.h"
#include "gl_utility.h" // gl_check_error
#include "glfw_adapter.h"

/* std includes */
#include <filesystem>
#include <cassert>

// glm includes
#include <glm/glm.hpp>

class shader : public glfw_adapter<GLuint> {
public:
	shader() = default;
	shader(const char* vertex_path, const char* fragment_path);
	~shader();
	void init(const char* vertex_path, const char* fragment_path);
	void use();
	void unuse();
	template <typename T>
	inline void load_uniform(const std::string& name, const T& uniform) const {
		assert(0);
	}
private:
	enum class SHADER_TYPE : uint32_t {
		VERTEX	 = 0,
		FRAGMENT = 1
	};
	GLuint load(const char*, const SHADER_TYPE type);
private:
};

// template specialization
template<>
inline void shader::load_uniform<glm::mat4>(const std::string& name, const glm::mat4& uniform) const {
	glUniformMatrix4fv(glGetUniformLocation(m_raw_attribute, name.c_str()), 1, GL_FALSE, &uniform[0][0]);
	check_error(__FILE__, __LINE__);
}

template<>
inline void shader::load_uniform<glm::vec3>(const std::string& name, const glm::vec3& uniform) const {
	glUniform3fv(glGetUniformLocation(m_raw_attribute, name.c_str()), 1, &uniform[0]);
	check_error(__FILE__, __LINE__);
}