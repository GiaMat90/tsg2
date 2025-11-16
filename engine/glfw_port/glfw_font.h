#pragma once

#include "font.h"
#include <string>

class glfw_font : public font {
public:
	glfw_font();
	glfw_font(const std::string& file_name, const color& c, const text_position& p, const std::size_t dim);
	virtual ~glfw_font();
	void load_font(const std::string& file_name, const std::size_t dim) override;
};