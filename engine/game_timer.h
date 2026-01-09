#pragma once

#include <geometry.h>

template<typename GameTimerImpl>
class game_timer {
public:
	using time_t = geometry::scalar;
public:
	game_timer(const unsigned fps) : m_fps(fps) {};
	virtual ~game_timer() = default;
	const time_t tick() { return static_cast<GameTimerImpl*>(this)->tick(); };
	void set_fps(const unsigned fps) { m_fps = fps; };
protected:
	unsigned m_fps{};
};