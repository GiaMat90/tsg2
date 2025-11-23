#ifdef GLFW_GAME 

#include "glfw_game_timer.h"
#include "gl_utility.h"
#include "gl_includes.h"

/* tsg includes */
#include <tsg/io.h>

glfw_game_timer::glfw_game_timer(const unsigned fps) : game_timer(fps) {
	glfwSetTime(0.0f);
	check_error(__FILE__, __LINE__);
}

const float glfw_game_timer::tick() {
	static float m_ticks_elapsed = glfwGetTime(); // convert in ms
	static const float tick_sec{ 1 / static_cast<float>(m_fps) }; // minimum time unit in ms
		while ( glfwGetTime() - m_ticks_elapsed < tick_sec){}
	// time elapsed
	float delta_time = (glfwGetTime() - m_ticks_elapsed);
	// store time for next tick
	m_ticks_elapsed = glfwGetTime();
	// upper limit the time
	const float threshold_sec{ 1.0f / m_fps };
	return delta_time > threshold_sec ? threshold_sec : delta_time;
}

#endif