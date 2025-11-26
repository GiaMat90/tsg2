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
}

arrow_and_bubbles::~arrow_and_bubbles() {
	tsg::logger::get_instance().write("arrow_and_bubbles dtor");
}

void arrow_and_bubbles::initialize() {
	create_physics();
	initialize_objects();
	m_state = GAME_STATE::RUNNING;
}

void arrow_and_bubbles::run_game() {
	while (GAME_STATE::RUNNING == m_state) {
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

void arrow_and_bubbles::initialize_objects() {
	// input engine stuff
	INCLUDE_ARROW(add_playable(&m_arrow));
	// physic engine stuff
	INCLUDE_ARROW(add_physical_object(&m_arrow));
	/* initialize objects */
	INCLUDE_BOUNDING_VOLUME(m_arrow.print_bounding_volume(true));
	INCLUDE_ARROW(add_drawable(&m_arrow));
	INCLUDE_ARROW(m_arrow.init());
	INCLUDE_BOUNDING_VOLUME(add_bounding_volume(m_arrow.get_bounding_volume(), m_arrow.get_sprite()->get_scale()));
	for (std::size_t i = 0u; i < NUMBER_OF_BUBBLE; ++i) {
		INCLUDE_BUBBLE(add_physical_object(&m_bubbles[i]));
		INCLUDE_BOUNDING_VOLUME(m_bubbles[i].print_bounding_volume(true));
		INCLUDE_BUBBLE(add_drawable(&m_bubbles[i]));
		INCLUDE_BUBBLE(m_bubbles[i].init());
		INCLUDE_BOUNDING_VOLUME(add_bounding_volume(m_bubbles[i].get_bounding_volume(), m_bubbles[i].get_sprite()->get_scale()));
	}
}

void arrow_and_bubbles::process_input() {
	if( GAME_EVENTS::QUIT == get_event()) {
		m_state = GAME_STATE::SHUT_DOWN;
	}
	m_input.process_input();
};

void arrow_and_bubbles::update_game() {
	auto tick = m_timer.tick();
	m_physics.update(tick);
}

void arrow_and_bubbles::generate_output() {
	m_renderer.render();
}