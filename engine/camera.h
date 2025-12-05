#pragma once

// engine includes
#include <window.h>
// tsg includes
#include <tsg/math.h>
// std includes
#include <bitset>

class camera_target {
protected:
	using position = tsg::vector<float, 3>;
public:
	camera_target() = default;
	~camera_target() = default;
	virtual position get_position() const = 0;
};

template <typename WindowImpl>
class camera {
public:
	enum class OPTIONS : std::size_t {
		FIXED = 0u,
		FOLLOW_TARGET,
		ZOOMABLE,
		MOUSE_CONTROLLED,
		KEY_CONTROLLED
	};
public:
	camera(WindowImpl* const w) : m_window(w) { assert(m_window); };
	virtual ~camera() = default;
	virtual void init() = 0;
	void set_target(camera_target* const t) { m_target = t; };
	void set_option(const OPTIONS opt) {
		m_options.set(static_cast<std::size_t>(opt));
	};
protected:
	WindowImpl* m_window{ nullptr };
	camera_target * m_target{ nullptr };
	std::bitset<32> m_options;
};