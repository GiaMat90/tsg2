#pragma once

#include "tsg2.h"
#include "game_object.h"
#include "geometry.h"

class drawable_bounding_volume : public texture_object {
public:
	drawable_bounding_volume(geometry::bounding_volume* bv = nullptr, const scalar scale = scalar(0));
	~drawable_bounding_volume();
	void init() override {};
	void update();
	inline void set_bounding_volume(geometry::bounding_volume * const bv) {
		assert(bv != nullptr);
		m_bv = bv;
	}
	inline geometry::bounding_volume * const get_bounding_volume() const { return m_bv; }
protected:
	geometry::bounding_volume* m_bv{ nullptr };
};