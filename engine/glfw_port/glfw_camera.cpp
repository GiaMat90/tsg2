#ifdef GLFW_GAME

#include "glfw_camera.h"
#include "gl_includes.h"
#include "gl_utility.h"
#include "input.h"
// tsg includes
#include <tsg/logger.h>
#include <tsg/io.h>
// std includes
#include <cassert>
// glm includes
#include <glm/gtc/matrix_transform.hpp>

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
static void mouse_callback_2D(GLFWwindow* window, double xposIn, double yposIn);
static void mouse_callback_3D(GLFWwindow* window, double xposIn, double yposIn);

class glfw_camera_data {
	friend camera<glfw_window, glfw_camera>;
	friend glfw_camera;
	friend void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	friend void mouse_callback_2D(GLFWwindow* window, double xposIn, double yposIn);
	friend void mouse_callback_3D(GLFWwindow* window, double xposIn, double yposIn);
	friend void mouse_2D();
private:
	// positions
	glm::vec3 m_position{};
	glm::vec3 m_front{};
	glm::vec3 m_up{};
	glm::vec3 m_right{};
	glm::vec3 m_world_up{};
	// euler angles
	float m_yaw{ 0.0f };
	float m_pitch{ 0.0f };
	// parameters
	float m_speed{ 10.0f };
	float m_sensitivity{ 0.0f };
	float m_zoom{ 1.0f };
	// window 
	glfw_window * m_window{ nullptr };
	glfw_camera* m_camera{ nullptr };
	// other
	bool m_capture_scroll_position{ false };
	bool m_scrolling{ false };
};

struct mouse_position {
	float x{ 0.0f };
	float y{ 0.0f };
};

static glfw_camera_data camera_data;
static mouse_position last_mouse_pos;

/* OpenGL callbacks */
static void capture_mouse_position(GLFWwindow*, double xposIn, double yposIn) {
	last_mouse_pos.x = static_cast<float>(xposIn);
	last_mouse_pos.y = static_cast<float>(yposIn);
}
/* Compute camera methods */
static void mouse_2D() {	
	/* Initial value */
	static float pos_x{ static_cast<float>(last_mouse_pos.x) };
	static float pos_y{ static_cast<float>(last_mouse_pos.y) };
	// 2D case
	camera_data.m_position.x += static_cast<float>(last_mouse_pos.x) - pos_x;
	camera_data.m_position.y -= static_cast<float>(last_mouse_pos.y) - pos_y;
	// update static variables
	pos_x = static_cast<float>(last_mouse_pos.x);
	pos_y = static_cast<float>(last_mouse_pos.y);
}
static void empty_mouse_callback(GLFWwindow*, double, double) {
	return; // do nothing
};
static void mouse_callback_2D(GLFWwindow*, double xposIn, double yposIn) {
	/* Initial value */
	static float pos_x{ static_cast<float>(xposIn) };
	static float pos_y{ static_cast<float>(yposIn) };
	// 2D case
	camera_data.m_position.x += static_cast<float>(xposIn) - pos_x;
	camera_data.m_position.y -= static_cast<float>(yposIn) - pos_y;
	// update static variables
	pos_x = static_cast<float>(xposIn);
	pos_y = static_cast<float>(yposIn);
};
static void mouse_callback_3D(GLFWwindow*, double xposIn, double yposIn) {
	/* Initial value */
	static float pos_x{ static_cast<float>(xposIn) };
	static float pos_y{ static_cast<float>(yposIn) };
	// compute process movement
	camera_data.m_yaw += static_cast<float>(xposIn) - pos_x;
	camera_data.m_pitch += static_cast<float>(yposIn) - pos_y;
	// computing the new front vector
	camera_data.m_front.x = cosf(glm::radians(camera_data.m_yaw)) * cosf(glm::radians(camera_data.m_pitch));
	camera_data.m_front.y = sinf(glm::radians(camera_data.m_pitch));
	camera_data.m_front.z = sinf(glm::radians(camera_data.m_yaw)) * cosf(glm::radians(camera_data.m_pitch));
	camera_data.m_front = glm::normalize(camera_data.m_front);
	// computing the new right vector
	camera_data.m_right = glm::normalize(glm::cross(camera_data.m_front, camera_data.m_world_up));
	// computing the new up vector
	camera_data.m_up = glm::normalize(glm::cross(camera_data.m_right, camera_data.m_front));
	// update static variables
	pos_x = static_cast<float>(xposIn);
	pos_y = static_cast<float>(yposIn);
}
static void scroll_callback(GLFWwindow*, double, double yoffset)
{
	camera_data.m_zoom -= static_cast<float>(yoffset) * 0.1f;
	// crop zoom value to a minimum
	if (camera_data.m_zoom < 0.1f) {
		camera_data.m_zoom = 0.1f;
	}
	if (camera_data.m_capture_scroll_position) {
		auto target_ndc_positin = camera_data.m_camera->screen_to_ndc(last_mouse_pos.x, last_mouse_pos.y);
		float k{ camera_data.m_sensitivity };
		camera_data.m_position.x += target_ndc_positin[geometry::axes::x] * k;
		camera_data.m_position.y -= target_ndc_positin[geometry::axes::y] * k;// Remember to reverse y-axes
	}
}
/* Camera static methods */
void camera<glfw_window, glfw_camera>::translate(const tsg::vector<float, 3>& v) {
	camera_data.m_position.x += v[0];
	camera_data.m_position.y += v[1];
	camera_data.m_position.z += v[2];
}
/* camera class method definitions */
glfw_camera::glfw_camera(glfw_window* const w) : camera(w) { 
	camera_data.m_window = w; 
	camera_data.m_camera = this;
}

glfw_camera::~glfw_camera() {}

void glfw_camera::init() const {
	glfwSetInputMode(m_window->cget_raw_attribute(), GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
	// make sure system sensible to mouse
	if(m_controls.test(static_cast<std::size_t>(camera_controls::follow_target))) {
		assert(m_target);
	}
	if (m_controls.test(static_cast<std::size_t>(camera_controls::scrollable_zoom))) {
		glfwSetScrollCallback(m_window->cget_raw_attribute(), scroll_callback);
	}
	if (m_controls.test(static_cast<std::size_t>(camera_controls::point_where_scrolling))) {
		camera_data.m_capture_scroll_position = true;
		glfwSetCursorPosCallback(m_window->cget_raw_attribute(), capture_mouse_position);
	}
}

void glfw_camera::update_camera(input_engine* const input) const {
	if(m_controls.test(static_cast<std::size_t>(camera_controls::follow_target))) {
		return;
	}
	if (m_controls.test(static_cast<std::size_t>(camera_controls::left_click_drag))) {
		if (input->is_mouse_pressed(input_mouse::left))
		{
			mouse_2D();
		}
		if(input->is_mouse_released(input_mouse::left))
		{
		}
	} 
	if (m_controls.test(static_cast<std::size_t>(camera_controls::wasd_controlled))) {
		// camera speed its thought as percentage of the window size
		if(input->is_key_pressed(input_key::key_w)) {
			camera_data.m_position.y -= camera_data.m_sensitivity;
		} 
		if (input->is_key_pressed(input_key::key_s)) {
			camera_data.m_position.y += camera_data.m_sensitivity;
		}
		if (input->is_key_pressed(input_key::key_a)) {
			camera_data.m_position.x -= camera_data.m_sensitivity;
		}
		if (input->is_key_pressed(input_key::key_d)) {
			camera_data.m_position.x += camera_data.m_sensitivity;
		} 
	}
}

geometry::vector2D glfw_camera::screen_to_ndc(const geometry::scalar x, const geometry::scalar y) const {
	assert(x < static_cast<geometry::scalar>(m_window->get_width()));
	assert(y < static_cast<geometry::scalar>(m_window->get_height()));
	// Convert screen coordinates to normalized device coordinates [-1, 1] X [-1, 1]
	const geometry::scalar ndc_x = (2.0f * x) / static_cast<geometry::scalar>(m_window->get_width()) - 1.0f;
	const geometry::scalar ndc_y = 1.0f - (2.0f * y) / static_cast<geometry::scalar>(m_window->get_height());
	return geometry::vector2D{ ndc_x, ndc_y };
}

geometry::vector2D glfw_camera::screen_to_world_2D(const geometry::scalar x, const geometry::scalar y) const {
	assert(x < static_cast<geometry::scalar>(m_window->get_width()));
	assert(y < static_cast<geometry::scalar>(m_window->get_height()));
	// Reverse the viewport transformation: from screen coordinates to NDC
	const geometry::scalar ndc_x = (2.0f * x) / static_cast<geometry::scalar>(m_window->get_width()) - 1.0f;
	const geometry::scalar ndc_y = 1.0f - (2.0f * y) / static_cast<geometry::scalar>(m_window->get_height());
	// Transform NDC to world coordinates
	auto world_position{ (glm::inverse(get_projection() * get_view())) * glm::vec4(ndc_x, ndc_y, 0.0f, 1.0f) };
	return geometry::vector2D{ world_position.x, world_position.y };
}

geometry::vector3D glfw_camera::screen_to_world_3D(const geometry::scalar x, const geometry::scalar y) const {
	/*TODO*/
	assert(0);    
	// Convert screen coordinates to normalized device coordinates [-1, 1]
	geometry::scalar depth = 0.0f; // Assuming a depth value of 0 for simplicity
	glm::vec4 ndc(
		(2.0f * x) / m_window->get_width() - 1.0f,
		1.0f - (2.0f * y) / m_window->get_height(), // Flip Y
		2.0f * depth - 1.0f,
		1.0f
	);
	// Transform through inverse projection and view
	glm::mat4 invViewProj = glm::inverse(get_projection() * get_view());
	glm::vec4 worldPos = invViewProj * ndc;
	// Perspective divide
	return geometry::vector3D({ worldPos.x / worldPos.w, worldPos.y / worldPos.w, worldPos.z / worldPos.w });
}

void glfw_camera::set_initial_zoom(const float z) const {
	camera_data.m_zoom = z;
}

void glfw_camera::set_sensitivity(const float s) const {
	camera_data.m_sensitivity = s;
}

glm::mat4 glfw_camera::get_view() const {
	auto view{ glm::mat4(1.0f) };
	switch (m_type) {
		case camera_type::fixed:
			// nothing to do here from camera side!
			break;
		case camera_type::ortographic:
			view = glm::translate(glm::mat4(1.0f), glm::vec3(-camera_data.m_position.x, -camera_data.m_position.y, -camera_data.m_position.z));
			break;
		case camera_type::perspective:
			/* view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp) where:
			* cameraPos is the position of the camera (x,y)
			* cameraFront is the direction the camera is looking at (0,0,-1) in NDC 
			* cameraUp is the up vector (0,1,0)
			* => in 2D case we need to set the camera position to (x,y,1) and look at (x,y,0) = position + front
			*/
			/* As per now this is well tested for 2D case. */
			if (m_target) {
				/* target should be already in NDC */
				assert((-1.0f <= m_target->get_target_position()[0]) && (m_target->get_target_position()[0] <= 1.0f));
				assert((-1.0f <= m_target->get_target_position()[1]) && (m_target->get_target_position()[1] <= 1.0f));
				assert((-1.0f <= m_target->get_target_position()[2]) && (m_target->get_target_position()[2] <= 1.0f));
				camera_data.m_position.x = m_target->get_target_position()[0];
				camera_data.m_position.y = m_target->get_target_position()[1];
				camera_data.m_position.z = m_target->get_target_position()[2];
				view = glm::lookAt(
					glm::vec3(camera_data.m_position.x, camera_data.m_position.y, 1.0f),
					glm::vec3(camera_data.m_position.x, camera_data.m_position.y, 0.0f),
					glm::vec3(0.0f, -1.0f, 0.0f)
				);
			}
			else {
				//return glm::lookAt(camera_data.m_position, camera_data.m_position + camera_data.m_front, camera_data.m_up); // <- 3D case
				view = glm::lookAt(
					glm::vec3(camera_data.m_position.x, camera_data.m_position.y, 1.0f),
					glm::vec3(camera_data.m_position.x, camera_data.m_position.y, 0.0f),
					glm::vec3(0.0f, -1.0f, 0.0f) // inverting the y-axes
				);
			}
			break;
		case camera_type::isometric:
			assert(0); // TODO
			break;
		case camera_type::undefined:
		default:
			assert(0); // camera type not defined
			break;
	}
	return view;
}

glm::mat4 glfw_camera::get_projection() const {
	//tsg::print("camera position: ({}, {})", camera_data.m_position.x, camera_data.m_position.y);
	auto projection{ glm::mat4(1.0f) };
	switch (m_type)
	{
	case camera_type::fixed:
		// nothing to do here from camera side!
		break;
	case camera_type::ortographic: {
		const float aspect{ static_cast<float>(m_window->get_width()) / static_cast<float>(m_window->get_height()) };

		// How much of the world do we see at once?
		const float zoom_factor{ camera_data.m_zoom };
		const float view_height{ zoom_factor}; 
		const float view_width{ view_height * aspect };

		// Camera shows a window into the world centered at camera position
		const float left{ camera_data.m_position.x - view_width / 2.0f };
		const float right{ camera_data.m_position.x + view_width / 2.0f };
		const float bottom{ camera_data.m_position.y - view_height / 2.0f };
		const float top{ camera_data.m_position.y + view_height / 2.0f };

		projection = glm::ortho(left, right, top, bottom, -1.0f, 100.0f);
	}
		break;
	case camera_type::perspective:
		// TODO: understand well the role of the last two parameters: its correct that they are in camera class?
		//return glm::perspective(glm::radians(45.0f), static_cast<float>(m_window->get_width()) / static_cast<float>(m_window->get_height()), 0.1f, 100.0f);
		projection = glm::perspective(glm::radians(camera_data.m_zoom), static_cast<float>(m_window->get_width()) / static_cast<float>(m_window->get_height()), 0.1f, 100.0f);
		break;
	case camera_type::isometric: {
		// Rotate to create isometric view		
		const float aspect{ static_cast<float>(m_window->get_width()) / static_cast<float>(m_window->get_height()) };

		// How much of the world do we see at once?
		const float zoomFactor{ 1.0f + (-camera_data.m_zoom / 180.0f) };
		const float viewHeight{ zoomFactor };  // See 20 units tall
		const float viewWidth{ viewHeight * aspect };

		// Camera shows a window into the world centered at camera position
		const float left{ camera_data.m_position.x - viewWidth / 2.0f };
		const float right{ camera_data.m_position.x + viewWidth / 2.0f };
		const float bottom{ camera_data.m_position.y - viewHeight / 2.0f };
		const float top{ camera_data.m_position.y + viewHeight / 2.0f };

		projection = glm::ortho(left, right, top, bottom, -1.0f, 100.0f);
		/* TODO: parametrize these rotation inputs */
		projection = glm::rotate(projection, glm::radians(35.264f), glm::vec3(1, 0, 0));
		projection = glm::rotate(projection, glm::radians(45.0f), glm::vec3(0, 1, 0));
	}
		break;
	case camera_type::undefined:
	default:
		assert(0); // not allowed
		break;
	}
	return projection;
}

#else
static_assert(false, "GLFW_GAME is not defined");
#endif