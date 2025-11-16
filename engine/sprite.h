#pragma once

#include "texture.h"

/* std includes */
#include <string>

class sprite : public virtual texture {
public:
	sprite(const std::string& path = "") {};
	virtual ~sprite() {};
public:
	virtual void load(const std::string& asset = "") = 0;
	virtual void unload() = 0;
public: // factory-method
	static sprite* create_sprite();
};
