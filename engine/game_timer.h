#pragma once

template<typename GameTimerImpl>
class game_timer {
public:
	game_timer(const unsigned fps) : m_fps(fps) {};
	virtual ~game_timer() = default;
	const float tick() { return static_cast<GameTimerImpl*>(this)->tick(); };
	void set_fps(const unsigned fps) { m_fps = fps; };
protected:
	unsigned m_fps{};
};