#pragma once

/* std includes */
#include <exception>
#include <string>

class window {
public:
	using width_t = int;
	using height_t = int;
public:
	class window_exception : public std::exception {
		const char* what() { return "window exception"; }
	};
	class create_exception : public window_exception {
		const char* what() { return "create exception"; }
	};
	struct position {
		width_t x{};
		height_t y{};
	};
	window(const std::string& title = "", const width_t w = 0u, const height_t h = 0u) :
		m_title(title), m_width(w), m_height(h) {}
	virtual ~window() = default;	
	inline height_t get_height() const { return m_height; }
	inline width_t get_width() const { return m_width; }
protected:
	std::string m_title;
	width_t m_width;
	height_t m_height;
	position m_start_pos;
};
