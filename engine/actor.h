#pragma once

class actor {
public:
	using life_t = unsigned int;
	struct features {
		life_t max_health{};
		life_t attack_power{};
		life_t defense{};
		life_t heal_power{};
	};
public:
	enum class State {
		Idle,
		Moving,
		Attacking,
		Dying,
		Dead
	};
public:
	actor() = default;
	actor(const features&, const life_t);
	virtual ~actor() = default;
public:
	void hit();
	void heal();
	inline void set_features(const features& f) { m_features = f; }
	inline features get_features() const { return m_features; }
	inline void set_max_health(const life_t h) { m_features.max_health = h; }
	inline void set_attack_power(const life_t p) { m_features.attack_power = p; }
	inline void set_heal_power(const life_t p) { m_features.heal_power = p; }
	inline void set_defense(const life_t d) { m_features.defense = d; }
	inline void set_state(const State state) { m_state = state; }
	inline State get_state() const { return m_state; }
	inline void set_health(const life_t h) { m_health = h; }
	inline life_t get_health() const { return m_health; }
protected:
	State m_state{ State::Idle };
	features m_features{};
	life_t m_health{ 0 };
};
