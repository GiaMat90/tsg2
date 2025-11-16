#pragma once

class color {
public:
	enum RGBA : unsigned {
		RED		= 0,
		GREEN	= 1,
		BLUE	= 2,
		ALPHA	= 3
	};
	color(unsigned r = 0u, unsigned g = 0u, unsigned b = 0u, unsigned a = 0u) {
		m_rgba[0] = r;
		m_rgba[1] = g;
		m_rgba[2] = b;
		m_rgba[3] = a;
	};
	inline unsigned& operator[](const RGBA value) { 
		return m_rgba[value]; 
	}
	inline const unsigned& operator[](const RGBA value) const {
		return m_rgba[value];
	}
	color(const color& other) {
		m_rgba[RGBA::RED] = other[RGBA::RED];
		m_rgba[RGBA::GREEN] = other[RGBA::GREEN];
		m_rgba[RGBA::BLUE] = other[RGBA::BLUE];
		m_rgba[RGBA::ALPHA] = other[RGBA::ALPHA];
	}
private:
	unsigned m_rgba[4u];
};