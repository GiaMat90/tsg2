#pragma once

#include <window.h>

template <typename WindowImpl>
class camera {
public:
	enum class TYPE {
		ZOOM_SCROLL
	};
public:
	camera(WindowImpl* const w) : m_window(w) {};
	virtual ~camera() = default;
protected:
	WindowImpl* m_window;
};