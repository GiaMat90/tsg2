#pragma once

#include "geometry.h"

/* tsg includes */
#include <tsg/types.h>

using geometry::scalar;

class texture {
public:
	using position = tsg::vector<scalar, 3>;
	using angle = scalar;
	using scale = scalar;
public:
	texture(int w = 0, int h = 0) : m_width(w), m_height(h) {};
	virtual ~texture() = default;
public:
	virtual void set_active() = 0;
public:
	/* set and getters */
	inline void set_size(int w = 0, int h = 0) { m_width = w; m_height = h; }
	inline void set_scale(const scale& s) { m_scale = s; }
	inline void set_where(const position& p) { m_position = p; }
	inline void set_rotation(const angle a) { m_angle = a; }
	inline scale get_scale() const { return m_scale; }
	inline position get_where() const { return m_position; }
	inline angle get_rotation() const { return m_angle; }
	inline position get_size() const { return m_scale * position({ scalar(m_width), scalar(m_height), scalar(m_depth) }); }
public: // factory-method
	static texture* create_texture();
protected:
	scale m_scale{1.0f};
	position m_position{};
	angle m_angle{};
	int m_width{};
	int m_height{};
	int m_depth{};
};