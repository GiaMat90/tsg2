#pragma once

#include "actor.h"

actor::actor(const features& f, const life_t h) : m_features(f), m_health(h) {}

void actor::hit() {
	if (m_health > m_features.defense) {
		m_health -= m_features.defense;
	} else {
		m_health = 0;
		m_state = state::dying;
	}
}

void actor::heal() {
	if (m_health + m_features.heal_power <= m_features.max_health) {
		m_health += m_features.heal_power;
	} else {
		m_health = m_features.max_health;
	}
}
