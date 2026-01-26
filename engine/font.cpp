#include "font.h"

font::font(const std::string& s, const color& c, const texture::position& p, const scale sc) : texture(p, sc),
		m_file_name(s), m_color(c) {};

font::font(const std::string& s, const color& c, const font_container& fc, const font_horizontal_alignment ha, const font_vertical_alignment va, const scale sc) : 
	texture(), m_file_name(s), m_color(c), m_ha(ha), m_va(va), m_container(fc)
{
	m_scale = sc;
};

//void font::set_text(const std::string& s) {
//	m_text = s;
//	compute_text_size();
//	compute_where();
//}

void font::compute_where() {
	switch (m_ha) {
	case font_horizontal_alignment::left:
		m_position[geometry::axes::x] = static_cast<scalar>(m_container.m_min_x);
		break;
	case font_horizontal_alignment::center:
		m_position[geometry::axes::x] = static_cast<scalar>((m_container.m_min_x + m_container.m_max_x - m_width)) / 2.0f;
		if (m_position[geometry::axes::x] <= m_container.m_min_x) {
			m_position[geometry::axes::x] = static_cast<scalar>(m_container.m_min_x);
		}
		break;
	case font_horizontal_alignment::right:
		m_position[geometry::axes::x] = static_cast<scalar>(m_container.m_max_x - m_width);
		break;
	default:
		// not allowed alignment
		assert(0);
		break;
	}
	switch (m_va)
	{
	case font_vertical_alignment::top:
		m_position[geometry::axes::y] = static_cast<scalar>(m_container.m_min_y);
		break;
	case font_vertical_alignment::center:
		m_position[geometry::axes::y] = static_cast<scalar>((m_container.m_min_y + m_container.m_max_y - m_height)) / 2.0f;
		if(m_position[geometry::axes::y] <= m_container.m_min_y) {
			m_position[geometry::axes::y] = static_cast<scalar>(m_container.m_min_y);
		}
		break;
	case font_vertical_alignment::bottom:
		m_position[geometry::axes::y] = static_cast<scalar>(m_container.m_max_y - m_height);
		break;
	default:
		// not allowed alignment
		assert(0);
		break;
	}
}
