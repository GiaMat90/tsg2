#pragma once

// engine includes
#include <window.h>
// std includes
#include <string>
#include <cassert>
// tsg includes
#include <tsg/math.h>

template <typename WindowImpl, typename CameraImpl, typename CursorImpl>
class cursor {
public:
	using screen_position = tsg::vector<float, 2>;
	using world_position = tsg::vector<float, 3>;
public:
	enum class event {
		none = 0,
		left_click = 1,
		right_click = 2,
		middle_click = 3,
		move = 4
	};
public:
	cursor(WindowImpl* const w, CameraImpl* const c, const std::string& image) : m_window(w), m_camera(c), m_image(image) { assert(m_window); assert(m_camera); };
	virtual ~cursor() = default;
public:
	inline void set_cursor_image(const std::string& image) { static_cast<CursorImpl*>(this)->set_cursor_image(image); m_image = image; };
	inline screen_position get_screen_position() const { return static_cast<CursorImpl*>(this)->get_screen_position(); };
	template <std::size_t Dim> requires (Dim == 2 || Dim == 3)
	inline tsg::vector<float, Dim> get_world_position() const { return static_cast<CursorImpl*>(this)->get_world_position<Dim>(); };
	inline event get_event() const { return static_cast<CursorImpl*>(this)->get_event(); };
protected:
	WindowImpl* m_window{ nullptr };
	CameraImpl* m_camera{ nullptr };
	std::string m_image{};
};