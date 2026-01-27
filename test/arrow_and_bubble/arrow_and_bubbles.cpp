#include "arrow_and_bubbles.h"
#include "game_event.h"
#include <tsg/logger.h>
#include <tsg/io.h>
#include <tsg/os.h>

#define EXCLUDE_BOUNDING_VOLUME 1

#define EXCLUDE_BUBBLE 0
#if !FORCE_INCLUDE_ARROW
#define EXCLUDE_ARROW NUMBER_OF_BUBBLE > 1 ? 1 : 0
#endif

#if EXCLUDE_BUBBLE
#define INCLUDE_BUBBLE( code ) /* code */
#else
#define INCLUDE_BUBBLE( code ) code
#endif

#if EXCLUDE_ARROW
#define INCLUDE_ARROW( code ) /* code */
#else
#define INCLUDE_ARROW( code ) code
#endif

#if EXCLUDE_BOUNDING_VOLUME
#define INCLUDE_BOUNDING_VOLUME( code ) /* code */
#else
#define INCLUDE_BOUNDING_VOLUME( code ) (code)
#endif


arrow_and_bubbles::arrow_and_bubbles(const std::string& window_text, const unsigned h, const unsigned w, const unsigned fps) :
	game(window_text, h, w, fps)
{
	tsg::logger::get_instance().write("arrow_and_bubbles ctor");
	m_bubbles.reserve(NUMBER_OF_BUBBLE);
}

arrow_and_bubbles::~arrow_and_bubbles() {
	tsg::logger::get_instance().write("arrow_and_bubbles dtor");
}

void arrow_and_bubbles::initialize() {
	create_physics();
	initialize_camera();
	set_cursor_image((tsg::os::get_exe_path() / std::filesystem::path("assets\\arrow.png")).string());

	initialize_objects();
	m_state = game_state::running;
}

void arrow_and_bubbles::run_game() {
	while (game_state::running == m_state) {
		process_input();
		update_game();
		generate_output();
	}
}

void arrow_and_bubbles::shutdown() {
	quit();
}

void arrow_and_bubbles::create_physics() {
	/* TODO: improve scale handling */
	geometry::scalar scale = 2.0f;
	m_physics.set_limits({ scalar(m_window.get_width()), scalar(m_window.get_height()) }, scale);
}

void arrow_and_bubbles::initialize_camera() {
	// camera stuff
	set_camera_type(camera_type::ortographic);
	set_camera_initial_zoom(1.0f);
	set_camera_sensitivity(0.1f);
	/* Mouse controls */ 
	set_camera_controls(camera_controls::scrollable_zoom);
	set_camera_controls(camera_controls::point_where_scrolling);
	/* Keyboard controls */
	set_camera_controls(camera_controls::wasd_controlled);
	camera_init();
}

void arrow_and_bubbles::initialize_objects() {
	set_cursor_image((tsg::os::get_exe_path() / std::filesystem::path("assets\\arrow.png")).string());
	// input engine stuff
	INCLUDE_ARROW(add_playable(&m_arrow));
	// physic engine stuff
	INCLUDE_ARROW(add_physical_object(&m_arrow));
	/* initialize objects */
	INCLUDE_ARROW(INCLUDE_BOUNDING_VOLUME(m_arrow.print_bounding_volume(true)));
	INCLUDE_ARROW(add_drawable(&m_arrow));
	INCLUDE_ARROW(m_arrow.init());
	INCLUDE_ARROW(INCLUDE_BOUNDING_VOLUME(add_bounding_volume(m_arrow.get_bounding_volume(), m_arrow.get_sprite()->get_scale())));
	for (std::size_t i = 0u; i < NUMBER_OF_BUBBLE; ++i) {
		INCLUDE_BUBBLE(m_bubbles.emplace_back());
		INCLUDE_BUBBLE(add_physical_object(&m_bubbles[i]));
		INCLUDE_BUBBLE(INCLUDE_BOUNDING_VOLUME(m_bubbles[i].print_bounding_volume(true)));
		INCLUDE_BUBBLE(add_drawable(&m_bubbles[i]));
		INCLUDE_BUBBLE(m_bubbles[i].init());
		INCLUDE_BUBBLE(INCLUDE_BOUNDING_VOLUME(add_bounding_volume(m_bubbles[i].get_bounding_volume(), m_bubbles[i].get_sprite()->get_scale())));
	}
}

void arrow_and_bubbles::process_input() {
	if( game_events::quit == get_event()) {
		m_state = game_state::shut_down;
	}
	m_input.process_input();
};

void arrow_and_bubbles::update_game() {
	const auto elapsed_time = tick();
	/* Actor interactions */
	bool update_engine_containers{ false };
	
	if (cursor_impl::event::left_click == get_cursor_event()){
		geometry::point2D cursor_pos = get_cursor_world_position();
		const auto victim = std::find_if(m_bubbles.begin(), m_bubbles.end(), [&](const bubble& b) {
			return b.get_bounding_volume()->contains(cursor_pos);
		});
		if(m_bubbles.end() != victim) {
			victim->set_state(actor::state::dead);
			victim->set_visible(drawable::state::invisible); // <- this hack works but the bubbles are still in the physics engine and in the drawable engine
			update_engine_containers = true;
		}
	}
	bool shutdown_game{ true };
	auto it{ m_bubbles.begin() };
	while(shutdown_game && it != m_bubbles.end()) {
		if (it->get_state() != actor::state::dead) {
			shutdown_game = false;
		}
		++it;
	}
	if (shutdown_game) {
		m_state = game_state::shut_down;
	}
	/* Update all objects */ // <- this doesn't works because the bounding volume result not istantiated yet when updating the physics
	if (false /*update_engine_containers*/) {
		auto it{ m_bubbles.begin() };
		while (it != m_bubbles.end()) {
			remove_physical_object(&*it);
			remove_drawable(&*it);
			++it;
		}
		std::erase_if(m_bubbles, [&](bubble& b) {
			return actor::state::dead == b.get_state();
			if (actor::state::dead == b.get_state()) {
				remove_physical_object(&b);
				remove_drawable(&b);
				return true;
			}
			else {
				return false;
			}
			});
		for (auto& b : m_bubbles) {
			add_physical_object(&b);
			add_drawable(&b);
		}
	}
	/**/
	update_physics(elapsed_time);
}

void arrow_and_bubbles::generate_output() {
	render_scene();
}