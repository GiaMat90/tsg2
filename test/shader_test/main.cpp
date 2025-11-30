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

#define GL_POLY_MODE 0
/* Choice what do you want draw */
#define DRAW_TEXTURE_VERTEXES 1
#define DRAW_LINE_VERTEXES    1
#define DRAW_REGPOLY_VERTEXES 1
#define DRAW_BOX2D_VERTEXES   1

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
	shader texture_shader(		
		(tsg::os::get_exe_path() / std::filesystem::path("shaders\\texture_vert.glsl")).string().c_str(),
		(tsg::os::get_exe_path() / std::filesystem::path("shaders\\texture_frag.glsl")).string().c_str()
	);
	check_glfw_error();

	shader line_shader(
		(tsg::os::get_exe_path() / std::filesystem::path("shaders\\line_vert.glsl")).string().c_str(),
		(tsg::os::get_exe_path() / std::filesystem::path("shaders\\line_frag.glsl")).string().c_str()
	);

	/**/
#if DRAW_TEXTURE_VERTEXES
	texture_vertex tex_vertex;
	tex_vertex.init();
#endif
	/**/
#if DRAW_LINE_VERTEXES
	line_vertex line(0.75f, 0.25f, 0.5f, 1.0f);
	line.init();
#endif
	/**/
#if DRAW_REGPOLY_VERTEXES
	regpoly_vertex<10000> polygon(0.75f, 0.75f, 0.55f, 1.0f);
	polygon.init();
#endif
#if DRAW_BOX2D_VERTEXES
	geometry::box<2> box2d({ 0.0f, 0.0f }, { 0.5f, 0.5f });
#endif
#if GL_POLY_MODE
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif
	while (false == glfwWindowShouldClose(window)) {
		process_input(window);  
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// initialize commons
		glm::mat4 transform{ glm::mat4(1.0f) }; // make sure to initialize matrix to identity matrix first
		float scale{ 1.0f };
		GLint transformLoc{ -1 };
		/* draw our first texture */
		texture_shader.use();
#if DRAW_TEXTURE_VERTEXES
		transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		scale = 0.5f;
		transform = glm::scale(transform, glm::vec3(scale, scale, scale));
		transform = glm::translate(transform, glm::vec3(0.5f, 0.5f, 0.0f));
		transform = glm::rotate(transform, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		transformLoc = glGetUniformLocation(texture_shader.get_adaptee_v(), "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
		check_error(__FILE__, __LINE__);
		// draw texture vertex
		tex_vertex.use();
		tex_vertex.draw();
#endif
		texture_shader.unuse();
		/* draw first line shader */
		line_shader.use();
#if DRAW_LINE_VERTEXES
		transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		scale = 0.5f;
		transform = glm::scale(transform, glm::vec3(scale, scale, scale));
		transform = glm::translate(transform, glm::vec3(-0.5f, -0.5f, 0.0f));
		transform = glm::rotate(transform, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		transformLoc = glGetUniformLocation(line_shader.get_adaptee_v(), "transform");
		check_error(__FILE__, __LINE__);
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
		check_error(__FILE__, __LINE__);
		// draw line vertex
		line.use();
		line.draw();
#endif
		// draw triangle using line shader
		transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		//scale = 1.0f;
		//transform = glm::scale(transform, glm::vec3(scale, scale, scale));
		//transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
		//transform = glm::rotate(transform, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		transformLoc = glGetUniformLocation(line_shader.get_adaptee_v(), "transform");
		check_error(__FILE__, __LINE__);
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
		check_error(__FILE__, __LINE__);
#if DRAW_REGPOLY_VERTEXES
		polygon.use();
		polygon.draw();
#endif
#if DRAW_BOX2D_VERTEXES
		box_vertex<2> box2d_vertex(&box2d, 0.5f, 0.75f, 0.25f, 1.0f);
		box2d_vertex.init();
		box2d_vertex.use();
		box2d_vertex.draw();		
		//box2d.translate({ 0.0001f, 0.0f }); // uncomment to see the box move runtime 
#endif
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