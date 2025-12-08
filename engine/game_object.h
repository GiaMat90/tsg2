#pragma once

#include "sprite.h"
#include "mesh.h"
#include "geometry.h"

/*
* Pure virtual base class for all game objects
* It shouldnt be inherited directly
*/
class game_object {
public:
	game_object() = default;
	virtual ~game_object() = default;
	virtual void init() = 0;
};

/*
* Drawable class base for all drawable game objects
* It shouldnt be inherited directly
*/
class drawable : public game_object {
public:
	drawable() = default;
	virtual ~drawable() = default;
	void print_bounding_volume(const bool print = false) { m_has_bounding_volume = print; }
	bool has_bounding_volume() const { return m_has_bounding_volume; }
protected:
	bool m_has_bounding_volume{ false };
};

/*
* Texture object class, it is a drawable object with a texture
*/
class texture_object : public drawable {
public:
	texture_object() = default;
	virtual ~texture_object() = default;
public:
	texture* get_texture() { return m_texture; }
	void set_texture(texture* r) { m_texture = r; }
protected:
	texture* m_texture{ nullptr };
};

/*
* Sprite object class, it is a drawable object with a sprite
* It should be inherited by all sprite based game objects
*/
class sprite_object : public drawable {
	using position = texture::position;
	using angle = texture::angle;
public:
	sprite_object();
	virtual ~sprite_object();
public:
	void update(const position&, const angle);
	void init() const;
public:
	inline sprite * const get_sprite() const { return m_sprite; }
	inline void set_sprite(sprite* s) { m_sprite = s; }
protected:
	sprite* m_sprite{ nullptr };
};

class mesh_object : public drawable {
public:
	mesh_object() = default;
	virtual ~mesh_object() = default;
public:
	mesh* get_mesh() { return m_mesh; }
	void set_mesh(mesh* m) { m_mesh = m; }
protected:
	mesh* m_mesh{ nullptr };
};


