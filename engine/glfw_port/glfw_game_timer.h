#pragma once

#include "game_timer.h"
#include "gl_includes.h"

class glfw_game_timer : public game_timer<glfw_game_timer>
{
public:
	glfw_game_timer(const unsigned fps);
	const float tick();

	//const float tick() {
	//	static float m_ticks_elapsed = glfwGetTime(); // convert in ms
	//	static const float tick_sec{ 1 / static_cast<float>(m_fps) }; // minimum time unit in ms
	//	while (glfwGetTime() - m_ticks_elapsed < tick_sec) {}
	//	// time elapsed
	//	float delta_time = (glfwGetTime() - m_ticks_elapsed);
	//	// store time for next tick
	//	m_ticks_elapsed = glfwGetTime();
	//	// upper limit the time
	//	const float threshold_sec{ 1.0f / m_fps };
	//	return delta_time > threshold_sec ? threshold_sec : delta_time;
	//};
};

