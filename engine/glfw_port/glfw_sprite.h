#pragma once

#include "sprite.h"
#include "glfw_texture.h"

class glfw_sprite : public virtual glfw_texture, public virtual sprite {
public:
	glfw_sprite(const std::string& asset = "", const int w = 0, const int h = 0)
		: glfw_texture(w, h), sprite(asset) {};
	virtual ~glfw_sprite() {}
public:
	void load(const std::string& asset = "") override;
	void unload() override;
};
