#pragma once

/* std includes */
#include <cstdint> // uint8_t

class color {
public:
	enum rgba : unsigned {
		red		= 0,
		green	= 1,
		blue	= 2,
		alpha	= 3,
	};
	color(uint8_t r = 0u, uint8_t g = 0u, uint8_t b = 0u, uint8_t a = 0u) {
		assert(r <= 255u);
		assert(g <= 255u);
		assert(b <= 255u);
		assert(a <= 255u);
		m_rgba[0] = r;
		m_rgba[1] = g;
		m_rgba[2] = b;
		m_rgba[3] = a;
	};
	color(const color& other) {
		m_rgba[rgba::red] = other[rgba::red];
		m_rgba[rgba::green] = other[rgba::green];
		m_rgba[rgba::blue] = other[rgba::blue];
		m_rgba[rgba::alpha] = other[rgba::alpha];
	}
	inline uint8_t& operator[](const rgba value) {
		assert(value <= rgba::alpha);
		return m_rgba[value]; 
	}
	inline const uint8_t& operator[](const rgba value) const {
		assert(value <= rgba::alpha);
		return m_rgba[value];
	}
	inline float get_normalized(const rgba value) const {
		assert(value <= rgba::alpha);
		return static_cast<float>(m_rgba[value]) / 255.0f;
	}
private:
	uint8_t m_rgba[4u];
};