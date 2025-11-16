#pragma once

enum class GAME_EVENTS : int {
	QUIT
};

template <typename WindowImpl, typename GameEventImpl>
class game_event {
public:
public:
	game_event(WindowImpl * w) : m_window(w) {};
public:
	GAME_EVENTS get_events() { return static_cast<GameEventImpl*>(this)->get_events(); };
protected:
	WindowImpl* m_window{ nullptr };
};
