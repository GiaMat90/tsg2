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

enum class font_horizontal_alignment : std::size_t {
	left,
	center,
	right
};

enum class font_vertical_alignment : std::size_t {
	top,
	center,
	bottom
};

class font; 

class font_container {
	friend font;
public:
	font_container(const std::size_t min_x = 0u, const std::size_t max_x = 0u, const std::size_t min_y = 0u, const std::size_t max_y = 0u) :
		m_min_x(min_x), m_max_x(max_x), m_min_y(min_y), m_max_y(max_y) {};
	virtual ~font_container() = default;
private:
	std::size_t m_min_x{};
	std::size_t m_max_x{};
	std::size_t m_min_y{};
	std::size_t m_max_y{};
};

class font : public texture, public std::string {
public:
	// ctors and dtors
	font() = default;
	font(const std::string& s, const color& c, const font_container& fc, const font_horizontal_alignment ha, const font_vertical_alignment va, const scale sc = 1.0f);
	font(const std::string& s, const color& c, const texture::position& p, const scale sc = 1.0f);
	virtual ~font() {};
public:
	// setters and getters
	inline void set_scale(const float s) { m_scale = s; };
	inline void set_text(const std::string& s) {
		m_text = s;
		if (false == m_text.empty()) {
			compute_text_size();
			compute_where();
		}
	}
	inline void set_container(const font_container& fc) {
		m_container = fc;
		if (false == m_text.empty()) {
			compute_text_size();
			compute_where();
		}
	}
	inline void add_row(const std::string& r) { m_rows.emplace_back(r); };
	inline void set_where(const texture::position& p) { m_position = p; }
	inline void set_color(const color& c) { m_color = c; }
	inline texture::position get_where() const { return m_position; }
	inline std::string get_text() const { return m_text; }
	inline color get_color() const { return m_color; }
	inline void set_alignmet(const font_horizontal_alignment ha, const font_vertical_alignment va) {
		m_ha = ha;
		m_va = va;
	};
	inline void set_horizontal_alignment(const font_horizontal_alignment ha) { m_ha = ha; };
	inline void set_vertical_alignment(const font_vertical_alignment va) { m_va = va; };
public: 
	// pure virtual public methods
	virtual void load_font(const std::size_t size = 12u, const font_type ft = font_type::arial, const font_style fs = font_style::regular) = 0;
public: 
	// factory-method
	static font* create_font();
protected: 
	// pure virtual protected methods
	virtual void load_font(const std::filesystem::path& file_name, const std::size_t dim = 12u) = 0;
	virtual void compute_text_size() = 0;
protected:
	void compute_where();
protected:
	std::string m_file_name{};
	std::string m_text{};
	std::vector<std::string> m_rows{};
	color m_color{};
	font_horizontal_alignment m_ha{font_horizontal_alignment::left};
	font_vertical_alignment m_va{font_vertical_alignment::top};
	font_container m_container{};
};