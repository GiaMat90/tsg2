#pragma once

#include <game_object.h>
#include <actor.h>
#include <physics.h>

class bubble : public actor, public sprite_object, public physics<2>::physical_object {
public:
	bubble();
	~bubble() = default;
public:
	void init() override;
	void update(const scalar delta_time) override;
};