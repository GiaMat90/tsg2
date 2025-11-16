#pragma once

#include "color.h"
/* tsg includes */
#include <tsg/math.h>
/* std includes */
#include <string>

class font {
protected:
	using text_position = tsg::vector<std::size_t, 2>;
public:
	font() = default;
	font(const std::string& s, const color& c, const text_position& p) : 
		m_file_name(s), m_color(c), m_position(p) {};
	virtual ~font() {};
	inline void set_text(const std::string& s) { m_text = s; };
	inline void set_where(const text_position& p) { m_position = p; }
	inline void set_color(const color& c) { m_color = c; }
	inline text_position get_where() const { return m_position; }
	inline std::string get_text() const { return m_text; }
	inline color get_color() const { return m_color; }
public: // pure virtual classes
	virtual void load_font(const std::string& file_name, const std::size_t dim = 12u) = 0;
public: // factory-method
	static font* create_font();
protected:
	std::string m_file_name{};
	std::string m_text{};
	text_position m_position{};
	color m_color{};
	font* m_font{};
};