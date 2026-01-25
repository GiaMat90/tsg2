#pragma once

#include "glfw_texture.h"
#include <font.h>
#include <string>
#include <filesystem>

class glfw_font : public font {
public:
	struct size {
		std::size_t width;
		std::size_t height;
	};
	struct pos {
		std::size_t x;
		std::size_t y;
	};
public:
	glfw_font();
	glfw_font(const std::string& file_name, const color& c, const texture::position& p, const std::size_t dim);
	virtual ~glfw_font();
	void load_font(const std::size_t size = 12u, const font_type ft = font_type::arial, const font_style fs = font_style::regular) override;
	void set_active() const override;
	void bind_char_texture(const char c) const;
	void cleanup();
	void compute_text_size(const std::string& s, const float scale = 1.0f);
	size get_text_size(const std::string& s, const float scale = 1.0f) const;	
	size get_char_size(const char c) const;
	pos get_char_pos(const char c) const;
private:
	void load_font(const std::filesystem::path& file_name, const std::size_t dim) override final;
};