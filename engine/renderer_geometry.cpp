#include "renderer_geometry.h"
	
drawable_bounding_volume::drawable_bounding_volume(geometry::bounding_volume * bv, const scalar scale) : m_bv(bv) {
	assert(m_bv != nullptr);
	m_texture = texture::create_texture();
	m_texture->set_scale(scale);
	assert(m_texture != nullptr);
}
drawable_bounding_volume::~drawable_bounding_volume() {
	if (m_texture) {
		delete m_texture;
	}
}
void drawable_bounding_volume::update() {
	switch (m_bv->get_type())
	{
	case geometry::bounding_volume::type::aabb:
	case geometry::bounding_volume::type::obb: {
		auto box = static_cast<geometry::box2D*>(m_bv);
		m_texture->set_where(texture::position({ box->get_center()[geometry::axes::x], box->get_center()[geometry::axes::y] }));
		break;
	}
	default:
		assert(0); // unsupported bounding volume type
		break;
	}
}