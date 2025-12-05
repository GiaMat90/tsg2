#pragma once

#include "geometry.h"
#include "sprite.h"
#include "texture.h"
#include "color.h"
#include "font.h"
#include "asset.h"
#include "game_object.h"	// drawable
#include "renderer_geometry.h"

/* std includes */
#include <exception>
#include <vector>

// forward declarations
class camera_target;

template <typename WindowImpl, typename RendererImpl, typename CameraImpl>
class renderer {
	using sprites = std::vector<sprite_object*>;
	using textures = std::vector<texture_object*>;
	using meshes = std::vector<mesh_object*>;
	//using bounding_volumes = std::vector<drawable_bounding_volume*>;
	using bounding_volumes = std::vector<geometry::bounding_volume*>;
public:
	class creation_exception : public std::exception {
		const char* what() { return "renderer creation exception"; }
	};
	renderer(WindowImpl * const w) : m_window(w), m_camera(w) {};
	virtual ~renderer() {
		/* TODO */
	};
public: 
	inline void render() { static_cast<RendererImpl*>(this)->render(); };
	inline void clear() { static_cast<RendererImpl*>(this)->clear(); };
	inline void set_draw_color(const color& c) { static_cast<RendererImpl*>(this)->set_draw_color(c); };
	inline void draw(sprite* s) { static_cast<RendererImpl*>(this)->draw(s); };
	inline void draw(texture* t) { static_cast<RendererImpl*>(this)->draw(t); };	
	inline void draw(mesh* m) { static_cast<RendererImpl*>(this)->draw(m); };
	inline void draw(font* f) { static_cast<RendererImpl*>(this)->draw(f); };
	inline void draw(geometry::bounding_volume* bv) { static_cast<RendererImpl*>(this)->draw(bv); };
	inline void draw(const geometry::box3D& b) { static_cast<RendererImpl*>(this)->draw(b); };
	inline void draw(const geometry::box2D& b) { static_cast<RendererImpl*>(this)->draw(b); };
	inline void draw(const drawable_bounding_volume& bdv) { static_cast<RendererImpl*>(this)->draw(bdv); };
	inline void render_bounding_volumes() {
		m_bounding_volume_rendering = true;
	}
public:
	// TODO: evalueate to made it private and friendable of game
	inline void add_drawable(drawable* const obj) {
		if (auto s = dynamic_cast<sprite_object*>(obj)) {
			m_sprites_obj.push_back(s);
		}
		else if (auto t = dynamic_cast<texture_object*>(obj)) {
			m_textures_obj.push_back(t);
		}
		else if (auto m = dynamic_cast<mesh_object*>(obj)) {
			m_meshes_obj.push_back(m);
		}
		else if(auto bv = dynamic_cast<drawable_bounding_volume*>(obj)) {
			//m_textures_obj.push_back(bv);
			assert(0);
		}
		else {
			throw std::runtime_error("Unsupported drawable type");
		}
	}
	inline void add_bounding_volume(geometry::bounding_volume* const bv, const scalar scale) {
		assert(bv);
		m_bv_obj.emplace_back(bv);
	}
	inline void add_camera(CameraImpl* const c) {
		assert(c);
	}
	inline void set_camera_target(camera_target* const t) {
		m_camera.set_target(t);
	}
	inline void set_camera_initial_zoom(const scalar z) {
		m_camera.set_initial_zoom(z);
	}
protected:
	WindowImpl* m_window;
	textures m_textures_obj;
	sprites m_sprites_obj;
	meshes m_meshes_obj;
	bounding_volumes m_bv_obj;
	bool m_bounding_volume_rendering{ false }; // <- evaluate to remove
	CameraImpl m_camera;
};
