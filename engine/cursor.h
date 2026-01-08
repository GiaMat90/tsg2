#pragma once

// engine includes
#include <window.h>
// std includes
#include <string>
#include <cassert>
// tsg includes
#include <tsg/math.h>

template <typename WindowImpl, typename CursorImpl>
class cursor {
public:
	using position = tsg::vector<float, 2>;
public:
	enum class event {
		none = 0,
		left_click = 1,
		right_click = 2,
		middle_click = 3,
		move = 4
	};
public:
	cursor(WindowImpl* const w, const std::string& image) : m_window(w), m_image(image) { assert(m_window); };
	virtual ~cursor() = default;
public:
	inline void set_cursor_image(const std::string& image) {
		static_cast<CursorImpl*>(this)->set_cursor_image(image);
		m_image = image;
	};
	inline position get_position() const { return static_cast<CursorImpl*>(this)->get_position(); };
	inline event get_event() const { return static_cast<CursorImpl*>(this)->get_event(); };
protected:
	WindowImpl* m_window{ nullptr };
	std::string m_image{};
};