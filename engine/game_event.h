#pragma once

#include "input.h"
// std includes
#include <cassert>
#include <unordered_map>

enum class game_events : int {
	quit,
	pause,
	main_menu,
	max_event = main_menu
};

template <typename WindowImpl, typename InputImpl, typename GameEventImpl>
class game_event {
public:
	game_event(WindowImpl * const w, InputImpl * const i) : m_window(w), m_input(i) 
	{
		assert(w);
		assert(i);
	};
public:
	game_events get_events() { return static_cast<GameEventImpl*>(this)->get_events(); };
	inline void set_key_event(game_events event, input_key key) {
		assert(event <= game_events::max_event);
		assert(key <= input_key::max_key);
		m_event_key_map[event] = key;
	};
protected:
	WindowImpl* m_window{ nullptr };
	InputImpl* m_input{ nullptr };
	std::unordered_map<game_events, input_key> m_event_key_map;
};
