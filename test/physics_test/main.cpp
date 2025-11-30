// engine
#include <glfw_port/gl_includes.h>
#include <glfw_port/shader.h>
#include <glfw_port/vertex.h>
#include <glfw_port/gl_utility.h>
#include <geometry.h>
// tsg
#include <tsg/logger.h>
#include <tsg/io.h>	// print
#include <cassert>
// glm
#include <linmath.h>	// mat4x4
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 800

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void check_glfw_error() {
	const char* description;
	int code = glfwGetError(&description);
	if (code > GLFW_NO_ERROR) {
		tsg::print("Error {}:{}", code, description);
		tsg::logger::get_instance().write("Error {}:{}", code, description);
	}
}

GLFWwindow* const init_window() {
	// initialize window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Create an GLFW Window
	GLFWwindow* window = glfwCreateWindow(800, 600, "ShaderTest", nullptr, nullptr);

	if (!window)
	{
		check_glfw_error();
		assert(0);
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	return window;
}

void process_input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main() {
	tsg::logger::get_instance().write();
	// before using any GLFW functions, the library must be initialized
	if(GLFW_FALSE == glfwInit()){
		return -1;
	}
	GLFWwindow* const window = init_window(); 
	check_glfw_error();
	if (GL_FALSE == gladLoadGL(glfwGetProcAddress)) {
		return -1;
	}
	shader line_shader(
		(tsg::os::get_exe_path() / std::filesystem::path("shaders\\line_vert.glsl")).string().c_str(),
		(tsg::os::get_exe_path() / std::filesystem::path("shaders\\line_frag.glsl")).string().c_str()
	);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (false == glfwWindowShouldClose(window)) {
		process_input(window);  
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// initialize commons
		glm::mat4 transform{ glm::mat4(1.0f) }; // make sure to initialize matrix to identity matrix first
		float scale{ 1.0f };
		GLint transformLoc{ -1 };
		/* draw first line shader */
		line_shader.use();
		geometry::box<2> box2d({ 0.0f, 0.0f }, { 0.5f, 0.5f });
		box_vertex<2> box2d_vertex(&box2d, 0.5f, 0.75f, 0.25f, 1.0f);
		box2d_vertex.init();
		box2d_vertex.use();
		box2d_vertex.draw();
		// unsuse line shader
		line_shader.unuse();
		/**/
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate(); 
	check_glfw_error();
	return 0;
}