#pragma once

#include "renderer.h"
#include "glfw_window.h"
#include "glfw_camera.h"

#include "shader.h"
#include "vertex.h"

class glfw_renderer : public renderer<glfw_window, glfw_renderer, glfw_camera>
{
public:
	template <typename S>
	class exception : public std::exception {
	public:
		exception(S s) : std::exception(), m_s(s) {};
		S what() { return m_s; }
	private:
		S m_s{};
	};
public:
	glfw_renderer(glfw_window * w);
	~glfw_renderer();
public: // overrided methods
	void render();
	void clear();
	void set_draw_color(const color&);
	void draw(texture* const);
	void draw(sprite* const);
	void draw(mesh*);
	void draw(font*);
	void draw(geometry::bounding_volume* const);
	void draw(geometry::box3D * const);
	void draw(geometry::box2D * const); 
	void draw(const drawable_bounding_volume&);
private:
	/* to render 2D texture */
	shader m_texture_shader;
	texture_vertex m_texture_vertex;
	/* to render 2D sprites */
	shader m_sprite_shader;
	texture_vertex m_sprite_vertex;
	/* to render 3D meshes */ 
	shader m_mesh_shader;
	mesh_vertex m_mesh_vertex;
	/* for bounding volumes */
	shader m_line_shader;
	line_vertex m_line_vertex;
};

