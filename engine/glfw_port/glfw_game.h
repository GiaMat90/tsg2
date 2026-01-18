#pragma once

#ifdef GLFW_GAME

class glfw_game {
public:
	glfw_game();
	~glfw_game();
protected:
	bool init();
	void quit();
};

#else
static_assert(false, "GLFW_GAME is not defined");
#endif