#pragma once

#include "geometry.h"
// std
#include <map>
#include <functional>

using geometry::scalar;
using geometry::axes;

template <std::size_t Dim> requires geometry::GeometricDimension<Dim>
class contact_engine {
	using resolve_map_t = std::map<
		std::pair<geometry::bounding_volume::type, geometry::bounding_volume::type>,
		std::function<void(const geometry::bounding_volume&, const geometry::bounding_volume&)>>;
	using vector = tsg::vector<scalar, Dim>;
	using point = tsg::vector<scalar, Dim>;
	using box = geometry::box<Dim>;
	using sphere = geometry::sphere<Dim>;
	using edges = geometry::box<Dim>::edges;
	struct contact_t {
		point m_point{};
		vector m_normal{};
		vector m_penetration_vector{};
		scalar m_penetration{};
	};
public:
	contact_engine() = default;
	~contact_engine() = default;
public:
	bool resolve(geometry::bounding_volume const * const first, geometry::bounding_volume const * const second) {
		const auto it = m_resolve_map.find(std::make_pair(first->get_type(), second->get_type()));
		if (it != m_resolve_map.end()) {
			it->second(*first, *second);
		}
		else {
			assert(0);
		}
		return m_has_contact;
	}
public:
	inline bool has_contact() const { return m_has_contact; }
public:
	/* getters result of computation */
	inline point get_point() const { return m_contact.m_point; }
	inline vector get_normal() const { return m_contact.m_normal; }
	inline scalar get_penetration() const { return m_contact.m_penetration; }
	inline vector get_penetration_vector() const { return m_contact.m_penetration_vector; }
protected:
	void compute(const box& fisrt, const sphere& second){
		/* TODO */
		assert(0);
	}; 
	void compute(const sphere& first, const sphere& second) {
		scalar sphere_distance{ point(first.get_center() - second.get_center()).get_norm() };
		scalar radius_sum{ first.get_radius() + second.get_radius() };
		if (tsg::pow(sphere_distance, 2) <= tsg::pow(radius_sum, 2)) {
			/* collision */
			/* TODO: resolve collision */
			m_has_contact = true;
			assert(0);
		}
		else {
			/* no collision */
			/* TODO */
			assert(0);
		}
	}
	void computeAABB(const box& first, const box& second) {
		if constexpr (Dim == 2) {
			if (first.get_max(axes::x) > second.get_min(axes::x)) {
				if ((first.get_max(axes::y) > second.get_min(axes::y)) &&
					(second.get_max(axes::y) > first.get_min(axes::y)))
				{
					/* collision! */
					/* TODO: resolve collision */
					m_has_contact = true;/* valid for AABB bounding volumes */
					scalar penetration_x = std::min(
						first.get_max(axes::x) - second.get_min(axes::x),
						second.get_max(axes::x) - first.get_min(axes::x)
					);
					scalar penetration_y = std::min(
						first.get_max(axes::y) - second.get_min(axes::y),
						second.get_max(axes::y) - first.get_min(axes::y)
					);
					/* Choose the minimum axes penetration */
					if (penetration_x < penetration_y) {
						/* X axes */
						m_contact.m_penetration = penetration_x;
						m_contact.m_normal = first.get_center()[axes::x] < second.get_center()[axes::x] ?
							vector({ scalar(-1), scalar(0) }) : vector({ scalar(1), scalar(0) });
						m_contact.m_penetration_vector = vector({ m_contact.m_normal[axes::x] * m_contact.m_penetration, scalar(0) });
					}
					else {
						/* Y axes */
						m_contact.m_penetration = penetration_y;
						m_contact.m_normal = first.get_center()[axes::y] < second.get_center()[axes::y] ?
							vector({ scalar(0), scalar(-1) }) : vector({ scalar(0), scalar(1) });
						m_contact.m_penetration_vector = vector({ m_contact.m_normal[axes::y] * m_contact.m_penetration, scalar(0) });
					}
					m_contact.m_point = (first.get_center() + second.get_center()) * scalar(0.5);
				}
				else {
					m_has_contact = false; // no collision
					return;
				}				
			}
			else {
				m_has_contact = false; // no collision
				return; 
			}
		}
		else if constexpr (Dim == 3) {
			assert(0); // TODO
		}
		else {
			/* Not allowed dimension */
			assert(0); 
		}
	}
	void computeOBB(const box& a, const box& b) {
		if constexpr (Dim == 2) {
			assert(0);
		}
		else if constexpr (Dim == 3) {
			/*
			* Vertex against Edges
			*/
			const auto& vertex_against_edge = [](box::vertexes v, box::edges e, std::vector<contact_t> c, bool swap) -> void {
				for (auto vertex_it = v.begin(); vertex_it != v.end(); ++vertex_it) {
					for (auto edge_it = e.begin(); edge_it != e.end(); ++edge_it) {
						/* compute the normal of a 2D vector is straightforward n(v(x,y)) = v(-y,x) */
						vector normal = { -edge_it->get_direction()[axes::y], edge_it->get_direction()[axes::x] };
						vector vertex_start = *vertex_it - edge_it->get_start();
						scalar distance = vector::dot(normal, vertex_start);
						if (distance < 0) {
							vector edge_dir = edge_it->get_direction();
							scalar projection = vector::dot(edge_dir, vertex_start);
							scalar edge_len = edge_it->get_lenght();
							if (projection >= 0u && projection <= edge_len) {
								c.emplace_back(edge_it->get_start() + (edge_dir * projection),
									swap ? scalar(-1) * normal : normal,
									geometry::abs(distance),
									c.m_normal * c.m_penetration);
							}
						}
					}
				}
			};
			/*
			* Edges against Edges
			*/
			const auto& edges_against_edges = [](const box& a, const box& b, std::vector<contact_t>& contacts) -> void {
				edges edges1{ a.get_edges() };
				edges edges2{ b.get_edges() };
				for (auto edg1_it = edges1.begin(); edg1_it != edges1.end(); ++edg1_it) {
					for (auto edg2_it = edges2.begin(); edg2_it != edges2.end(); ++edg2_it) {
						/* Skip computation for almost parallel edges */
						constexpr scalar parallel_threshold{ 0.95 };
						if (vector::dot(edg1_it->get_direction(), edg2_it->get_direction()) > parallel_threshold) {
							continue;
						}
						else {
							auto [point_edg1, point_edg2] = edg1_it->closest_to(*edg2_it);
							scalar d = distance(point_edg1, point_edg2);
							/* Normal have to points from obb1 to obb2 */
							if (d < geometry::scalar_zero) {
								vector normal = vector::cross(edg1_it->get_direction(), edg2_it->get_direction());
								vector center2center = b.get_center() - a.get_center();
								if (vector::dot(normal, center2center) < scalar(0)) {
									normal *= scalar(-1);
								}
								scalar penetration{};
								/* Compute edge-edge penetration */
								{
									point center_edge1 = edg1_it->get_direction() * edg1_it->get_lenght() * scalar(0.5f);
									point center_edge2 = edg2_it->get_direction() * edg2_it->get_lenght() * scalar(0.5f);
									if (a.contains(center_edge2) || b.contains(center_edge1)) {
										penetration = (point_edg1 - point_edg2).get_norm();
									}
								}
								if (penetration > scalar(0)) {
									contacts.emplace_back(scalar(0.5) * (point_edg1 + point_edg2), normal, penetration, normal * penetration);
								}
							}
						}
					}
				}
			};			
			/* checking a-vertexes against b-edges */
			std::vector<contact_t> contacts{};
			vertex_against_edge(a.get_vertexes(), b.get_edges(), contacts, false);
			/* checking b-vertexes against a-edges */
			vertex_against_edge(b.get_vertexes(), a.get_edges(), contacts, true);
			/* checking edges against edges */
			edges_against_edges(a, b, contacts);
			/* early return */
			if (contacts.size() < 0u) {
				return;
			}
			/* compute best contact based on the greater penetration */
			std::sort(contacts.begin(), contacts.end(), [](const contact_t& c1, const contact_t& c2) -> bool { return c1.m_penetration > c2.m_penetration; });
			m_contact = contacts.front();
		}
		else {
			/* Not allowed dimension */
			assert(0);
		}
	}
	void computeAABBOBB(const box& a, const box& b) {
		assert(0); // TODO
	}
private:
	bool m_has_contact{ false };
	contact_t m_contact;
	const resolve_map_t m_resolve_map
	{
		{
			{geometry::bounding_volume::type::aabb, geometry::bounding_volume::type::aabb},
			[&](const geometry::bounding_volume& l, const geometry::bounding_volume& r) -> void { computeAABB(static_cast<const box&>(l), static_cast<const box&>(r)); }
		},
		{
			{geometry::bounding_volume::type::obb, geometry::bounding_volume::type::obb},
			[&](const geometry::bounding_volume& l, const geometry::bounding_volume& r) -> void { computeOBB(static_cast<const box&>(l), static_cast<const box&>(r)); }
		},
		{
			{geometry::bounding_volume::type::aabb, geometry::bounding_volume::type::obb},
			[&](const geometry::bounding_volume& l, const geometry::bounding_volume& r) -> void { computeAABBOBB(static_cast<const box&>(l), static_cast<const box&>(r)); }
		},
		{
			{geometry::bounding_volume::type::obb, geometry::bounding_volume::type::aabb},
			[&](const geometry::bounding_volume& l, const geometry::bounding_volume& r) -> void { computeAABBOBB(static_cast<const box&>(r), static_cast<const box&>(l)); }
		},
		{
			{geometry::bounding_volume::type::aabb, geometry::bounding_volume::type::sphere},
			[&](const geometry::bounding_volume& l, const geometry::bounding_volume& r) -> void { compute(static_cast<const box&>(l), static_cast<const sphere&>(r)); }
		},
		{
			{geometry::bounding_volume::type::obb, geometry::bounding_volume::type::sphere},
			[&](const geometry::bounding_volume& l, const geometry::bounding_volume& r) -> void { compute(static_cast<const box&>(l), static_cast<const sphere&>(r)); }
		},
		{
			{geometry::bounding_volume::type::sphere, geometry::bounding_volume::type::sphere},
			[&](const geometry::bounding_volume& l, const geometry::bounding_volume& r) -> void { compute(static_cast<const sphere&>(l), static_cast<const sphere&>(r)); }
		}
	};
};