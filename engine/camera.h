#pragma once

// engine includes
#include <window.h>
#include <geometry.h>
// tsg includes
#include <tsg/math.h>
// std includes
#include <bitset>

class input_engine;

class camera_target {
protected:
	using position = tsg::vector<float, 3>;
public:
	camera_target() = default;
	~camera_target() = default;
	virtual position get_target_position() const = 0;
};

enum class camera_type : std::size_t {
	undefined = 0u,
	fixed,
	ortographic,
	perspective,
	isometric,
	max_type = isometric
};

enum class camera_controls : std::size_t {
	follow_target,
	scrollable_zoom,
	point_where_scrolling,
	left_click_drag, // <- TODO: improve
	wasd_controlled,
};

template <typename WindowImpl, typename CameraImpl>
class camera {
public:
	camera(WindowImpl* const w, const camera_type t = camera_type::undefined) : m_window(w), m_type(t) {
		assert(m_window); 
	};
	virtual ~camera() = default;
public:
	// static callbacks
	static void translate(const tsg::vector<float, 3>& v);
public:
	inline void init() const { static_cast<CameraImpl*>(this)->init(); }
	inline void update_camera(input_engine* const i) const { static_cast<CameraImpl* const>(this)->update_camera(i); }
	template<std::size_t Dim> requires (Dim == 2 || Dim == 3)
	inline tsg::vector<geometry::scalar, Dim> screen_to_world(const geometry::scalar x, const geometry::scalar y) const { return static_cast<CameraImpl*>(this)->screen_to_world<Dim>(x,y); }
public:
	// setters
	inline void set_initial_zoom(const float z) const { static_cast<CameraImpl*>(this)->set_initial_zoom(z); };
	inline void set_target(camera_target* const t) { assert(t);  m_target = t; };
	inline void set_type(const camera_type t) { m_type = t; };
	inline void set_controls(const camera_controls opt) { m_controls.set(static_cast<std::size_t>(opt)); };
protected:
	WindowImpl* m_window{ nullptr };
	camera_target * m_target{ nullptr };
	camera_type m_type{ camera_type::undefined };
	std::bitset<32> m_controls;
};