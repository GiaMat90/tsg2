#pragma once

#include "game_object.h"
#include "contact.h"
#include "geometry.h"
// tsg includes
#include <tsg/io.h>
#include <tsg/types.h>
// std includes
#include <vector>
#include <algorithm>
#include <utility>

#define FREE_WORLD 1

using geometry::AXES;
using geometry::scalar;
using geometry::quaternion;
using geometry::matrix3D;
using geometry::bounding_volume;

template <std::size_t Dim> requires geometry::GeometricDimension<Dim>
class physics {
	/* Generic definitions */
	using vector = tsg::vector<scalar, Dim>;
	using point = tsg::vector<scalar, Dim>;
	using box = geometry::box<Dim>;
	using contact_engine = contact_engine<Dim>;
public:
	/* Support classes */
	/* Class of physical world that determine limits, determine contacs and resolve them. */
	class physical_object; // forward declaration
	class physical_world {
		friend physics;
	public:
		physical_world() = default;
		~physical_world() = default;
	public:
		inline void compute() {
			if constexpr (Dim == 3) {
				/* TODO */
				assert(0);
				/* Sorting objects x-based */
				std::sort(m_objects.begin(), m_objects.end(), [](physical_object* a, physical_object* b)
					{
						return a->get_bounding_volume()->get_min(geometry::X) < b->get_bounding_volume()->get_min(geometry::X);
					}
				);
				/* First check if there is contact with world walls */
				for (auto it = m_objects.begin(); it != m_objects.end(); ++it) {
					auto obj = *it;
					auto wall_contact = [&](const vector& normal) {
							scalar seperataing_velocity{ vector::dot((obj->m_velocity), normal) };
							vector impulse{ (-2 * seperataing_velocity / obj->m_inverse_mass) * normal };
							obj->m_velocity += obj->m_inverse_mass * impulse;
						};
					// compute if the new position is inside the world, else translate it
					if (obj->m_box.get_max(AXES::X) > m_limits.get_max(AXES::X)) {
						if (!obj->m_velocity.is_zero()) {
							scalar dx = obj->get_bounding_volume()->get_max(AXES::X) - m_limits.get_max(AXES::X);
							scalar dy{ dx * obj->m_velocity.get<AXES::Y>() / obj->m_velocity.get<AXES::X>() };
							scalar dz{ dx * obj->m_velocity.get<AXES::Z>() / obj->m_velocity.get<AXES::X>() };
							obj->translate({ -dx, -dy, -dz });
							
						}
						wall_contact({ scalar(-1), scalar(0), scalar(0) });
						return;
					}
					if (obj->m_box.get_max(AXES::Y) > m_limits.get_max(AXES::Y)) {
						if (!obj->m_velocity.is_zero()) {
							scalar dy = obj->get_bounding_volume()->get_max(AXES::Y) - m_limits.get_max(AXES::Y);
							scalar dx{ dy * obj->m_velocity.get<AXES::Y>() / obj->m_velocity.get<AXES::Y>() };
							scalar dz{ dy * obj->m_velocity.get<AXES::Z>() / obj->m_velocity.get<AXES::Y>() };
							obj->translate({ -dx, -dy, -dz });
							
						}
						wall_contact({ scalar(0), scalar(-1), scalar(0) });
						return;
					}
					if (obj->m_box.get_max(AXES::Z) > m_limits.get_max(AXES::Z)) {
						/* Nothing to do for now */
					}
					//
					if (obj->m_box.get_min(AXES::X) < m_limits.get_min(AXES::X)) {
						if (!obj->m_velocity.is_zero()) {
							scalar dx = obj->get_bounding_volume()->get_min(AXES::X) - m_limits.get_min(AXES::X);
							scalar dy{ dx * obj->m_velocity.get<AXES::Y>() / obj->m_velocity.get<AXES::X>() };
							scalar dz{ dx * obj->m_velocity.get<AXES::Z>() / obj->m_velocity.get<AXES::X>() };
							obj->translate({ -dx, -dy, -dz });							
						}
						wall_contact({ scalar(1), scalar(0), scalar(0) });
						return;
					}
					if (obj->m_box.get_min(AXES::Y) < m_limits.get_min(AXES::Y)) {
						if (!obj->m_velocity.is_zero()) {
							scalar dy = obj->get_bounding_volume()->get_min(AXES::Y) - m_limits.get_min(AXES::Y);
							scalar dx{ dy * obj->m_velocity.get<AXES::Y>() / obj->m_velocity.get<AXES::Y>() };
							scalar dz{ dy * obj->m_velocity.get<AXES::Z>() / obj->m_velocity.get<AXES::Y>() };
							obj->translate({ -dx, -dy, -dz });							
						}
						wall_contact({ scalar(0), scalar(1), scalar(0) });
						return;
					}
					if (obj->m_box.get_min(AXES::Z) < m_limits.get_min(AXES::Z)) {
						if (!obj->m_velocity.is_zero()) {
							scalar dz = obj->get_bounding_volume()->get_min(AXES::Z) - m_limits.get_min(AXES::Z);
							scalar dx{ dz * obj->m_velocity.get<AXES::X>() / obj->m_velocity.get<AXES::Z>() };
							scalar dy{ dz * obj->m_velocity.get<AXES::Y>() / obj->m_velocity.get<AXES::Z>() };
							obj->translate({ -dx, -dy, -dz });
							
						}
						wall_contact({ scalar(0), scalar(0), scalar(1) });
						return;
					}
					/* Search contacts with next objects */
					auto next_it = std::next(it);
					while (next_it != m_objects.end()) {
						auto next_obj = *next_it;
						if (m_contact_engine.resolve(obj->get_bounding_volume(), next_obj->get_bounding_volume())) {
							resolve_contact(obj, next_obj);
							++next_it;
						}
						else {
							next_it = m_objects.end();
						}
					}
				}
			}
			else if constexpr (Dim == 2) {
				/* Sorting objects x-based */
				std::sort(m_objects.begin(), m_objects.end(), [](physical_object* a, physical_object* b) 
					{
						return a->get_bounding_volume()->get_min(geometry::X) < b->get_bounding_volume()->get_min(geometry::X);
					}
				);
				/* First check if there is contact with world walls */
				for (auto it = m_objects.begin(); it != m_objects.end(); ++it) {
					auto obj = *it;
#if !FREE_WORLD
					auto wall_contact = [&](const vector& normal) {
							scalar seperataing_velocity{ vector::dot((obj->m_velocity), normal) };
							vector impulse{ (-2 * seperataing_velocity / obj->m_inverse_mass) * normal };
							obj->m_velocity += obj->m_inverse_mass * impulse;
						};
					// compute if the object collide with a wall (world limit)
					if (obj->get_bounding_volume()->get_max(AXES::X) > m_limits.get_max(AXES::X)) {
						if (!obj->m_velocity.is_zero()) {
							scalar dx = obj->get_bounding_volume()->get_max(AXES::X) - m_limits.get_max(AXES::X);
							scalar dy{ dx * obj->m_velocity.get<AXES::Y>() / obj->m_velocity.get<AXES::X>() };
							obj->translate({ -dx, -dy });						
						}
						wall_contact({ scalar(-1), scalar(0) });
						return;
					}
					if (obj->get_bounding_volume()->get_max(AXES::Y) > m_limits.get_max(AXES::Y)) {
						if (!obj->m_velocity.is_zero()) {
							scalar dy = obj->get_bounding_volume()->get_max(AXES::Y) - m_limits.get_max(AXES::Y);
							scalar dx{ dy * obj->m_velocity.get<AXES::Y>() / obj->m_velocity.get<AXES::Y>() };
							obj->translate({ -dx, -dy });
						}
						wall_contact({ scalar(0), scalar(-1) });
						return;
					}
					if (obj->get_bounding_volume()->get_min(AXES::X) < m_limits.get_min(AXES::X)) {
						if (!obj->m_velocity.is_zero()) {
							scalar dx = obj->get_bounding_volume()->get_min(AXES::X) - m_limits.get_min(AXES::X);
							scalar dy{ dx * obj->m_velocity.get<AXES::Y>() / obj->m_velocity.get<AXES::X>() };
							obj->translate({ -dx, -dy });							
						}
						wall_contact({ scalar(1), scalar(0) });
						return;
					}
					if (obj->get_bounding_volume()->get_min(AXES::Y) < m_limits.get_min(AXES::Y)) {
						if (!obj->m_velocity.is_zero()) {
							scalar dy = obj->get_bounding_volume()->get_min(AXES::Y) - m_limits.get_min(AXES::Y);
							scalar dx{ dy * obj->m_velocity.get<AXES::Y>() / obj->m_velocity.get<AXES::Y>() };
							obj->translate({ -dx, -dy });							
						}
						wall_contact({ scalar(0), scalar(1) });
						return;
					}
#endif
					/* Search contacts with next objects */
					auto next_it = std::next(it);
					while (next_it != m_objects.end()) {
						auto next_obj = *next_it;
						if (m_contact_engine.resolve(obj->get_bounding_volume(), next_obj->get_bounding_volume())) {
							resolve_contact(obj, next_obj);
						}
						++next_it;
					}
				}
			}
			else {
				/* Not allowed dimension */
				assert(0);
			}
		};
		inline vector get_scale() const { return m_scale; }
	protected:
		inline void resolve_contact(physics::physical_object* const a, physics::physical_object* const b) {
			scalar total_inverse_mass{ a->m_inverse_mass + b->m_inverse_mass };
			/* resolve velocity */
			scalar separating_velocity{ vector::dot((a->m_velocity - b->m_velocity), m_contact_engine.get_normal()) };
			if (separating_velocity > scalar(0)) {
				// objects are separating, no need to resolve
				return;
			}
			/* Linear coefficient */
			/* case with restitution coeff variable */
			constexpr scalar restutition_coeff{ scalar(1) };
			scalar new_separating_velocity{ -separating_velocity * restutition_coeff };
			scalar delta_velocity{ new_separating_velocity - separating_velocity };
			vector impulse{ (delta_velocity / total_inverse_mass) * m_contact_engine.get_normal() };
			a->m_velocity += a->m_inverse_mass * impulse;
			b->m_velocity -= b->m_inverse_mass * impulse;
			/* Angular coefficient */
			if constexpr (Dim == 3) {
				assert(0); // TODO
			}
			else if constexpr (Dim == 2) {
				auto tmp = m_contact_engine.get_point() - a->m_position;
				auto opposite_impulse = scalar(-1) * impulse;
				const scalar torque_a = tsg::vector<scalar, 3>::cross(geometry::point3D(tmp), geometry::point3D(opposite_impulse))[AXES::Z];
				tmp = m_contact_engine.get_point() - b->m_position;
				const scalar torque_b = tsg::vector<scalar, 3>::cross(geometry::point3D(tmp), geometry::point3D(opposite_impulse))[AXES::Z];
				a->m_angular_speed += torque_a;
				b->m_angular_speed -= torque_b;
			}
		};
	protected:
		std::vector<physical_object*> m_objects;
		box m_limits;
		vector m_scale;
		vector m_forces;
		contact_engine m_contact_engine;
	};
	/* physical_object to compute motion and collisions */
	class physical_object {
		friend physics;
	public:
		virtual ~physical_object() {
			if (m_bounding_volume) {
				delete m_bounding_volume;
				m_bounding_volume = nullptr;
			}
		};
	public:
		inline void set_physical_world(physical_world* const world) { m_world = world; }
	public:
		// updateable method overrides
		virtual inline void update(const scalar delta_time) {
			/*
			* Consume the acceleration due to forces
			*/
			m_velocity += m_acceleration * delta_time;
			/*
			* Now the position changes due only the velocity.
			* Update as a translation.
			*/
			translate(m_velocity * delta_time);
			/*
			* Clear accumulation
			*/
			m_acceleration.zero();
			/*
			* Update orientation and rotation
			* TODO: check if it should be converted to deg
			*/
			m_rotation += (m_angular_speed * delta_time);
			/*
			* Check if something goes very wrong
			*/
			if (m_position.is_nan()) {
				tsg::logger::get_instance().write("Exception in {}: physical object computation fails to get a number.", __FILE__);
				assert(0);
			}
		};
	public:
		inline void translate(const point& t) {
			m_position.translate(t);
			m_bounding_volume->translate(t);
		}
		inline void rotate(const scalar angle) {
			m_rotation += angle;
			m_bounding_volume->rotate(angle);
		}
	public:
		/* Bounding volume stuff */
		template <geometry::bounding_volume::type T, typename ...Args>
		inline void set_bounding_volume(Args... args) {
			if constexpr (T == bounding_volume::type::aabb || T == bounding_volume::type::obb) {
				m_bounding_volume = new geometry::box<Dim>(args...);
			}
			else if constexpr (T == bounding_volume::type::sphere) {
				m_bounding_volume = new geometry::sphere<Dim>(args...);
			}
			else if constexpr (T == bounding_volume::type::polygon) {
				m_bounding_volume = new geometry::polygon<Dim>(args...);
			}
			else {
				/* No supported bounding volume type */
				assert(0);
			}
		};
		inline void set_bounding_volume(const bounding_volume& other) { *m_bounding_volume = other; };
		bounding_volume * const get_bounding_volume() { return m_bounding_volume; }
		inline void set_mass(const scalar m) {
			if (m > scalar(0)) {
				m_inverse_mass = scalar(1) / m;
			}
			else {
				assert(false);
			}
		};
	public:
		inline scalar get_mass() const { return scalar(1) / m_inverse_mass; }
		inline void set_infinite_mass() { m_inverse_mass = scalar(0); }
	protected:
		inline void go_forward() { /* TODO */ assert(0); }
		inline void go_backward() { /* TODO */ assert(0); }
		inline void go_left() { /* TODO */ assert(0); }
		inline void go_right() { /* TODO */ assert(0); }
		inline void go_up() { /* TODO */ assert(0); }
		inline void go_down() { /* TODO */ assert(0); }
		inline void stop() { m_velocity.zero(); m_acceleration.zero(); }
		inline void push(const vector& force) {
			m_acceleration += m_inverse_mass * force;
		}
	protected:
		// world where the object lives
		physical_world* m_world{ nullptr };
		// boundaries
		bounding_volume* m_bounding_volume{ nullptr };
		// linear proprieties
		vector m_position{};
		vector m_velocity{};
		vector m_acceleration{};
		scalar m_inverse_mass{scalar(1)};
		// angular proprietis
		quaternion m_orientation{};
		vector m_angular_velocity{};
		vector m_angular_acceleration{};
		matrix3D m_inverse_intertia_tensor{};
		scalar m_rotation{};
		scalar m_angular_speed{};
		// accumulators
		vector m_forces{};
		vector m_torques{};
		vector m_impulse{};
		// dampings
		scalar m_linear_damping{};
		scalar m_angular_damping{};
	};
public:
	/* ctors and dtors methods */
	physics() {
		m_world = new physical_world();
	};
	~physics() {
		if (m_world) {
			delete m_world;
			m_world = nullptr;
		}
	}
public:
	// set proprieties
	void set_limits(const vector& half_sizes, const scalar scale = scalar(1.0f)) {
		/*
		* in 3D case: { 1.0f / (0.5f * width) , 1.0f / (0.5f * height), 1.0f / (0.5f * depth) } 
		* in 2D case: { 1.0f / (0.5f * width) , 1.0f / (0.5f * height) } 
		*/
		if (auto res = half_sizes.reciprocal()) {
			m_world->m_scale = res.value() * scalar(2.0f) * scale;
		}
		else {
			assert(0);
		}
		/* setting world limits */
		m_world->m_limits.set_center(vector(scalar(0)));
		m_world->m_limits.set_half_sizes(vector(scale));
		tsg::logger::get_instance().write("World limits: ({},{},{},{}) with scale ({},{})", 
			m_world->m_limits.get_center()[AXES::X] - m_world->m_limits.get_sizes()[AXES::X],
			m_world->m_limits.get_center()[AXES::X] + m_world->m_limits.get_sizes()[AXES::X],
			m_world->m_limits.get_center()[AXES::Y] + m_world->m_limits.get_sizes()[AXES::Y],
			m_world->m_limits.get_center()[AXES::Y] - m_world->m_limits.get_sizes()[AXES::Y],
			m_world->m_scale[AXES::X],
			m_world->m_scale[AXES::Y]);
	}
public:
	inline void update(const float delta_time) {
		for (auto o : m_world->m_objects) {
			o->update(delta_time);
		}
		m_world->compute();
	}
public:
	// TODO: evalueate to made it private and friendable of game
	inline void add_physical_object(physical_object* o) {
		o->m_world = m_world;
		m_world->m_objects.push_back(o);
	}
protected:
	physical_world* m_world{ nullptr };
};

