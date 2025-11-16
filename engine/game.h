#pragma once

/* tsg2 includes */
#include "input.h"
#include "input_object.h"
#include "game_timer.h"
#include "texture.h"
#include "physics.h"
#include "game_event.h"

/* tsg includes */
#include <tsg/types.h>

/* Graphic impl includes */
#ifdef GLFW_GAME
#include "glfw_port/glfw_port.h"
#elifdef VULKAN_GAME
/* TODO */
#include "vulkan_port/vulkan_port.h"
#elifdef GLES_GAME
/* TODO */
#include "gles_port/gles_port.h"
#endif

template <size_t Dim, typename GameImpl, typename GraphicImpl = graphic_impl >
class game : public tsg::non_copyable, public GraphicImpl
{
	using physics = physics<Dim>;
public:
	enum class GAME_STATE {
		NONE,
		INITIALIZING,
		RUNNING,
		STOPPING,
		SHUT_DOWN
	};
	game(const std::string& txt, const unsigned h, const unsigned w, const unsigned fps) : 
		GraphicImpl(), m_window(txt, h, w), m_renderer(&m_window), m_input(&m_window), m_event(&m_window), m_timer(fps), m_physics() {
		
	};
	virtual ~game() {}
	void initialize() { static_cast<GameImpl*>(this)->initialize(); };
	void run_game() { static_cast<GameImpl*>(this)->run_game(); };
	void shutdown() { static_cast<GameImpl*>(this)->shutdown(); };
public:
	window * const get_window() const { return &m_window; }
	GAME_EVENTS get_event() { return m_event.get_events(); };
protected: // virtual methods
	void process_input() { static_cast<GameImpl*>(this)->process_input(); };
	/* Calculate elapsed time and update game-object states */
	void update_game() { static_cast<GameImpl*>(this)->update_game(); };
	/* Create video output */
	void generate_output() { static_cast<GameImpl*>(this)->generate_output(); };
	/* Set game physics proprieties */
	void create_physics() { static_cast<GameImpl*>(this)->create_physics(); };
	/* initialize objects */
	void initialize_objects() { static_cast<GameImpl*>(this)->initialize_objects(); };
protected:
	void set_fps(const unsigned fps) { m_timer.set_fps(fps); }
public:
	inline void add_drawable(drawable* const obj) { m_renderer.add_drawable(obj); };
	inline void add_bounding_volume(geometry::bounding_volume* const bv, const scalar scale) { m_renderer.add_bounding_volume(bv, scale); };
	inline void add_playable(playable_object* const o) { m_input.add_playable(o); };
	inline void add_physical_object(physics::physical_object* const o) { m_physics.add_physical_object(o); }
protected: // attributes
	GAME_STATE m_state{ GAME_STATE::NONE };
	window_impl m_window;
	renderer_impl m_renderer;
	input_impl m_input;
	game_event_impl m_event;
	game_timer_impl m_timer;
	physics m_physics;
};

