#include "arrow.h"
#include <texture.h>
#include <tsg/os.h>

using geometry::scalar;
using geometry::cos;

arrow::arrow() : sprite_object(), physics<2>::physical_object() {}

void arrow::init() {
	m_sprite->load((tsg::os::get_exe_path() / std::filesystem::path("assets\\arrow.png")).string());
	m_sprite->set_where(texture::position({ m_position[geometry::AXES::X], m_position[geometry::AXES::Y]}));
	m_sprite->set_scale(0.5f);
	auto w = m_sprite->get_size().get<geometry::AXES::X>();
	auto h = m_sprite->get_size().get<geometry::AXES::Y>();
	geometry::vector2D half_sizes{ w / scalar(2), h / scalar(2) };
	half_sizes.scale(m_world->get_scale());
	set_bounding_volume<geometry::bounding_volume::type::aabb>(m_position, half_sizes);
	set_mass(scalar(1));
}

void arrow::update(const scalar delta_time) {
	physical_object::update(delta_time); // update physics, position and rotation
	sprite_object::update(m_position, m_rotation);
}

void arrow::process_input(input_engine* const input) {
	//m_velocity.zero();
	if (input->is_key_pressed(INPUT_KEY::KEY_W)) {
		/* Go up */
		push({ scalar(0), scalar(1) });
	}
	if (input->is_key_pressed(INPUT_KEY::KEY_A)) {
		/* Go left */
		push({ scalar(-1), scalar(0) });
	}
	if (input->is_key_pressed(INPUT_KEY::KEY_S)) {
		/* Go down */
		push({ scalar(0), scalar(-1) });
	}
	if (input->is_key_pressed(INPUT_KEY::KEY_D)) {
		/* Go right */
		push({ scalar(1), scalar(0) });
	}

	constexpr geometry::scalar rotation_unit{ 0.05f };
	if (input->is_key_pressed(INPUT_KEY::KEY_LEFT)) {
		/* Rotate left */
		m_rotation += rotation_unit;
	}
	if (input->is_key_pressed(INPUT_KEY::KEY_RIGHT)) {
		/* Rotate right */
		m_rotation += -rotation_unit;
	}
	if (input->is_key_pressed(INPUT_KEY::KEY_UP)) {
		/* Rotate right */
		push({ geometry::cos(m_rotation), geometry::sin(m_rotation) });
	}
	if (input->is_key_pressed(INPUT_KEY::KEY_DOWN)) {
		/* Rotate right */
		push({ -geometry::cos(m_rotation), -geometry::sin(m_rotation) });
	}

	if (input->is_key_pressed(INPUT_KEY::KEY_SPACE)) {
		/* JUMP */
	}
};