#pragma once

#include "glfw_texture.h"
#include <font.h>
#include <string>
#include <filesystem>

class glfw_font : public font {
public:
	struct size_t {
		std::size_t width;
		std::size_t height;
	};
	struct pos_t {
		std::size_t x;
		std::size_t y;
	};
	struct char_data_t {
		size_t size;       // Size of glyph
		pos_t pos;
	};
public:
	glfw_font();
	glfw_font(const std::string& file_name, const color& c, const texture::position& p, const std::size_t dim);
	virtual ~glfw_font();
	void load_font(const std::size_t size = 12u, const font_type ft = font_type::arial, const font_style fs = font_style::regular) override;
	void set_active() const override;
	void bind_char_texture(const std::size_t i) const;
	void bind_char_texture(const char c) const;
	void cleanup();
	size_t get_text_size() const;
	decltype(m_text)::size_type str_size() const { return m_text.size(); }
	char_data_t get_character_data(const std::size_t i) const;
private:
	size_t get_char_size(const char c) const;
	pos_t get_char_pos(const char c) const;
	void load_font(const std::filesystem::path& file_name, const std::size_t dim) override final;
	void compute_text_size() override final;
};