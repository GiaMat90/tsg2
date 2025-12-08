#include "game_object.h"

sprite_object::sprite_object() {
	m_sprite = sprite::create_sprite();
}

sprite_object::~sprite_object() {
	if (m_sprite) {
		delete m_sprite;
		m_sprite = nullptr;
	}
}

void sprite_object::init() const {
	assert(m_sprite);
}

void sprite_object::update(const position& p, const angle a) {
	if (m_sprite) {
		m_sprite->set_where(p);
		m_sprite->set_rotation(a);
	}
}

