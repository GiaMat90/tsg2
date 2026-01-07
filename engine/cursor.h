#pragma once

// engine includes
#include <window.h>
// std includes
#include <string>
#include <cassert>

template <typename WindowImpl, typename CursorImpl>
class cursor {
public:
	cursor(WindowImpl* const w, const std::string& image) : m_window(w), m_image(image) { assert(m_window); };
	virtual ~cursor() = default;
public:
	inline void set_cursor_image(const std::string& image) {
		static_cast<CursorImpl*>(this)->set_cursor_image(image);
		m_image = image;
	};
protected:
	WindowImpl* m_window{ nullptr };
	std::string m_image{};
};