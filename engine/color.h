#pragma once

/* std includes */
#include <cstdint> // uint8_t

class color {
public:
	enum class predefined : uint32_t {
		white	= 0xffffffff,
		black	= 0x000000ff,
		red		= 0xff0000ff,
		green	= 0x00ff00ff,
		blue	= 0x0000ffff,
		yellow	= 0xffff00ff,
		cyan	= 0x00ffffff,
		magenta = 0xff00ffff,
	};
public:
	enum rgba : unsigned {
		red		= 0,
		green	= 1,
		blue	= 2,
		alpha	= 3,
	};
	color() {
		m_rgba[0] = 0u;
		m_rgba[1] = 0u;
		m_rgba[2] = 0u;
		m_rgba[3] = 255u;
	};
	color(const predefined rgba_value) {
		m_rgba[0] = (static_cast<uint32_t>(rgba_value) >> 24u) & 0xffu;
		m_rgba[1] = (static_cast<uint32_t>(rgba_value) >> 16u) & 0xffu;
		m_rgba[2] = (static_cast<uint32_t>(rgba_value) >> 8u) & 0xffu;
		m_rgba[3] = static_cast<uint32_t>(rgba_value) & 0xffu;
	};
	color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
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