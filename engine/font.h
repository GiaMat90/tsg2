#pragma once

#include "texture.h"
#include "color.h"
/* tsg includes */
#include <tsg/math.h>
/* std includes */
#include <string>

enum class font_type : std::size_t {
	arial,
	carlito,
	courier_new,
	times_new_roman,
	verdana,
	unknown
};

enum class font_style : std::size_t {
	regular,
	bold,
	italic
};

class font : public texture {
public:
	font() = default;
	font(const std::string& s, const color& c, const texture::position& p, const scale sc = 1.0f) : texture(p, sc),
		m_file_name(s), m_color(c) {};
	virtual ~font() {};
	inline void set_scale(const float s) { m_scale = s; };
	inline void set_text(const std::string& s) { m_text = s; };
	inline void set_where(const texture::position& p) { m_position = p; }
	inline void set_color(const color& c) { m_color = c; }
	inline texture::position get_where() const { return m_position; }
	inline std::string get_text() const { return m_text; }
	inline color get_color() const { return m_color; }
public:
	virtual void load_font(const std::size_t size = 12u, const font_type ft = font_type::arial, const font_style fs = font_style::regular) = 0;
public: // factory-method
	static font* create_font();
protected: // pure virtual classes
	virtual void load_font(const std::filesystem::path& file_name, const std::size_t dim = 12u) = 0;
protected:
	std::string m_file_name{};
	std::string m_text{};
	color m_color{};
	//text_position m_position{};
	//scale m_scale{ 1.0f };
};