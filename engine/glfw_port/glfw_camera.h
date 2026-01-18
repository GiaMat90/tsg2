#pragma once

#ifdef GLFW_GAME

#include <camera.h>
#include "glfw_window.h"
#include <glm/glm.hpp>

class glfw_renderer;
class glfw_cursor;

class glfw_camera : public camera<glfw_window, glfw_camera> {
	friend glfw_renderer;
	friend glfw_cursor;
public:
	glfw_camera(glfw_window * const);
	virtual ~glfw_camera();
	void init() const;
	void update_camera(input_engine* const) const;
	void set_initial_zoom(const float z) const;
	template<std::size_t Dim> requires (Dim == 2 || Dim == 3)
	inline tsg::vector<geometry::scalar, Dim> screen_to_world(const geometry::scalar x, const geometry::scalar y) {
		if constexpr (Dim == 2) {
			return screen_to_world_2D(x, y);
		}
		else if constexpr (Dim == 3) {
			return screen_to_world_3D(x, y);
		}
		else {
			assert(0); // not allowed dimension
		}
	}
private:
	geometry::vector2D screen_to_world_2D(const geometry::scalar x, const geometry::scalar y);
	geometry::vector3D screen_to_world_3D(const geometry::scalar x, const geometry::scalar y);
private:
	glm::mat4 get_view() const;
	glm::mat4 get_projection() const;

};

#else
static_assert(false, "GLFW_GAME is not defined");
#endif