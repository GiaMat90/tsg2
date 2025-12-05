#pragma once

#include <window.h>
#include <tsg/math.h>

template <typename WindowImpl>
class camera {
	using vector = tsg::vector<float, 3>;
public:
	enum class TYPE {
		FIXED,
		MOUSE_CONTROLLED,
		KEY_CONTROLLED
	};
public:
	camera(WindowImpl* const w) : m_window(w) { assert(m_window); };
	virtual ~camera() = default;
	virtual void init() = 0;
protected:
	WindowImpl* m_window{ nullptr };
};