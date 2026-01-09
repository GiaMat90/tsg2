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

enum class CAMERA_OPTIONS : std::size_t {
	DIMENSION2,
	DIMENSION3,
	FIXED = 0u,
	FOLLOW_TARGET,
	MOUSE_CONTROLLED,	
	SCROLLABLE_ZOOM,
	LEFT_CLICK_DRAG, // <- TODO: improve
	KEY_CONTROLLED,
	WASD_CONTROLLED,
	MAX_OPTION = KEY_CONTROLLED
};

template <typename WindowImpl, typename CameraImpl>
class camera {
public:
	camera(WindowImpl* const w) : m_window(w) { assert(m_window); };
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
	inline void set_option(const CAMERA_OPTIONS opt) {
		m_options.set(static_cast<std::size_t>(opt));
	};
protected:
	WindowImpl* m_window{ nullptr };
	camera_target * m_target{ nullptr };
	std::bitset<32> m_options;
};