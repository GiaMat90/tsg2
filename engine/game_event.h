#pragma once

#include "input.h"
// std includes
#include <cassert>
#include <unordered_map>

enum class GAME_EVENTS : int {
	QUIT,
	PAUSE,
	MAIN_MENU,
	MAX_EVENT = MAIN_MENU
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
	GAME_EVENTS get_events() { return static_cast<GameEventImpl*>(this)->get_events(); };
	inline void set_key_event(GAME_EVENTS event, INPUT_KEY key) {
		assert(event <= GAME_EVENTS::MAX_EVENT);
		assert(key <= INPUT_KEY::MAX_KEY);
		m_event_key_map[event] = key;
	};
protected:
	WindowImpl* m_window{ nullptr };
	InputImpl* m_input{ nullptr };
	std::unordered_map<GAME_EVENTS, INPUT_KEY> m_event_key_map;
};
