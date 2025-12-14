#include "arrow.h"
#include <texture.h>
#include <tsg/os.h>

#define MOUSE_DRIVEN 1
#define MOUSE_DRAG 0

using geometry::scalar;
using geometry::cos;

arrow::arrow() : sprite_object(), physics<2>::physical_object(), camera_target() {}

void arrow::init() {
	sprite_object::init();
	physical_object::init();
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
	/* I don't want the arrow be subject the same physics than the bubbles */
	//physical_object::update(delta_time); // update physics, position and rotation
	constexpr scalar rotation{ 3.0f*static_cast<float>(std::numbers::pi) / 4.5f }; // rad
	sprite_object::update(m_position, rotation);
}

void arrow::process_input(input_engine* const input) {
#if MOUSE_DRIVEN
	/*/
	float x{}, y{};
	input->get_mouse_position(x, y);
	tsg::print("mouse pos ({},{})", x, y);
	/**/
	//constexpr geometry::scalar sensitivity{ 2.0f }; // TODO(maybe): adjust sensitivity based on the zoom level 
#if MOUSE_DRAG
	static geometry::vector2D offset{};
	static bool dragging{ false };
	if(input->is_mouse_pressed(INPUT_MOUSE::LEFT)) {
		dragging = true;
		//float x{}, y{};
		//input->get_mouse_position(x, y);
		//tsg::logger::get_instance().write("Dragging started at ({},{})", x, y);
	} else if (input->is_mouse_released(INPUT_MOUSE::LEFT)) {
		if(dragging) {
			// compute new offset
			input->get_mouse_position(offset[geometry::AXES::X], offset[geometry::AXES::Y]);
			offset[geometry::AXES::Y] *= scalar(-1); // invert Y axis
			offset = m_position - offset;
			//m_position -= offset;
			//tsg::logger::get_instance().write("Dragging ended. New offset: ({},{})", offset[geometry::AXES::X], offset[geometry::AXES::Y]);
			dragging = false;
		}
		else {
			input->get_mouse_position(m_position[geometry::AXES::X], m_position[geometry::AXES::Y]);
			m_position[geometry::AXES::Y] *= scalar(-1); // invert Y axis
			m_position += offset;
			//tsg::logger::get_instance().write("Arrow position: ({},{})", m_position[geometry::AXES::X], m_position[geometry::AXES::Y]);
		}
	}
#else 
	input->get_mouse_position(m_position[geometry::AXES::X], m_position[geometry::AXES::Y]);
	m_position[geometry::AXES::Y] *= scalar(-1); // invert Y axis

	if(input->is_mouse_pressed(INPUT_MOUSE::LEFT)) {
		actor::set_state(actor::State::Attacking);
	}
#endif
#else
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
#endif
};

camera_target::position arrow::get_target_position() const {
	return tsg::vector<scalar, 3>(m_position);
}