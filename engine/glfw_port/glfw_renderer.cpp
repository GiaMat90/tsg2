#ifdef GLFW_GAME 

#include "glfw_renderer.h"
#include "glfw_texture.h"
#include "glfw_sprite.h"
#ifdef TODO 
#include "glfw_mesh.h"
#endif
#include "glfw_font.h"
#include "gl_utility.h"	// gl_check_error
#include "geometry.h"

/* tsg includes */
#include <tsg/logger.h>
#include <tsg/io.h>	// print

/* glm includes */
#include <linmath.h>	// mat4x4
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using geometry::AXES;

glfw_renderer::glfw_renderer(glfw_window * const w, glfw_camera * const c) : renderer<glfw_window, glfw_renderer, glfw_camera>(w, c) {
	// NOTE: OpenGL error checks have been omitted for brevity
	if (0 == gladLoadGL(glfwGetProcAddress)) {
		throw exception("gladLoadGL Error");
	};
	glfwSwapInterval(1);

	m_texture_shader.init(
		(tsg::os::get_exe_path() / std::filesystem::path("shaders\\texture_vert.glsl")).string().c_str(),
		(tsg::os::get_exe_path() / std::filesystem::path("shaders\\texture_frag.glsl")).string().c_str()
	);
	m_texture_vertex.init();
	
	m_sprite_shader.init(
		(tsg::os::get_exe_path() / std::filesystem::path("shaders\\texture_vert.glsl")).string().c_str(),
		(tsg::os::get_exe_path() / std::filesystem::path("shaders\\texture_frag.glsl")).string().c_str()
	);
	m_sprite_vertex.init();

	m_line_shader.init(
		(tsg::os::get_exe_path() / std::filesystem::path("shaders\\line_vert.glsl")).string().c_str(),
		(tsg::os::get_exe_path() / std::filesystem::path("shaders\\line_frag.glsl")).string().c_str()
	);
	m_line_vertex.init();
}

glfw_renderer::~glfw_renderer() { /* Nothing to do */ }

// overrided methods
void glfw_renderer::render() {
	/* Render here */
	check_error(__FILE__, __LINE__);
	int width, height;
	glfwGetFramebufferSize(m_window->get_adaptee_r(), &width, &height);
	const float ratio = width / static_cast<float>(height);

	glViewport(0, 0, width, height);
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);		// petrolio
	//glClearColor(0.86f, 0.86f, 0.86f, 1.0f);	// gray
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_DEPTH_TEST);
	check_error(__FILE__, __LINE__);
	
	/* draw textures */
	if (m_textures_obj.size() > 0) {
		// use programs previously loaded
		m_texture_shader.use();
		// use vertexes previously loaded
		m_texture_vertex.use();
		for (const auto& t : m_textures_obj) {
			if (t->is_visible()) {
				this->draw(t->get_texture());
			}
		}
	}
	/* drawing sprites */
	if (m_sprites_obj.size() > 0) {
		// use programs previously loaded
		m_sprite_shader.use();
		m_sprite_shader.load_uniform("projection", m_camera->get_projection());
		m_sprite_shader.load_uniform("view", m_camera->get_view());
		// use vertexes previously loaded
		m_sprite_vertex.use();
		for (const auto& s : m_sprites_obj) {
			if (s->is_visible()) {
				this->draw(s->get_sprite());
			}
		}
		m_sprite_shader.unuse();
	}
	/* drawing meshes */
	if (m_meshes_obj.size() > 0) {
		/* TODO */
		assert(0);
		// use programs previously loaded
		m_mesh_shader.use();
		// use vertexes previously loaded
		m_mesh_vertex.use();
		for (const auto& m : m_meshes_obj) {
			if (m->is_visible()) {
				this->draw(m->get_mesh());
			}
		}
		m_mesh_shader.unuse();
	}
	/* drawing bounding volumes */
	if (m_bv_obj.size() > 0) {
		//// use programs previously loaded
		m_line_shader.use();
		m_line_shader.load_uniform("view", m_camera->get_projection());
		m_line_shader.load_uniform("projection", m_camera->get_view());
		for (const auto& bv : m_bv_obj) {
			this->draw(bv);
		}
		m_line_shader.unuse();
	}
	/* Swap front and back buffers */
	glfwSwapBuffers(m_window->get_adaptee_r());
	check_error(__FILE__, __LINE__);
}

void glfw_renderer::clear() {
	/* TODO */
	assert(0);
}

void glfw_renderer::set_draw_color(const color& c)
{
	/* TODO */
	assert(0);
}

void glfw_renderer::draw(texture const * const t) {
	if (auto texture = dynamic_cast<glfw_texture const *>(t)) {
		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		model = glm::scale(model, glm::vec3(texture->get_scale(), texture->get_scale(), 1.0f)); // <- old, TOREMOVE
		model = glm::translate(model, glm::vec3(texture->get_where()[AXES::X], texture->get_where()[AXES::Y], texture->get_where()[AXES::Z]));
		model = glm::rotate(model, texture->get_rotation(), glm::vec3(0.0f, 0.0f, 1.0f));

		if (glIsTexture(texture->get_adaptee_v()) != GL_TRUE) {
			assert(0); // texture is not a valid OpenGL texture
		}

		unsigned int transformLoc = glGetUniformLocation(m_texture_shader.get_adaptee_v(), "model");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));
		check_error(__FILE__, __LINE__);

		glBindTexture(GL_TEXTURE_2D, texture->get_adaptee_v());
		check_error(__FILE__, __LINE__);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		check_error(__FILE__, __LINE__);
	}
	else {
		tsg::logger::get_instance().write(tsg::string("{},{}: Error casting texture", __FILE__, __LINE__));
		assert(0); // texture is not a glfw_texture
	}
}

void glfw_renderer::draw(sprite * const s) {
	if (auto sprite = dynamic_cast<glfw_sprite*>(s)) {
		assert(glIsTexture(sprite->get_adaptee_v()));

		sprite->set_active();

		GLint boundTexture;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexture);
		assert(boundTexture == sprite->get_adaptee_v());

		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		//model = glm::scale(model, glm::vec3(sprite->get_scale(), sprite->get_scale(), 1.0f));
		model = glm::translate(model, glm::vec3(sprite->get_where()[AXES::X], sprite->get_where()[AXES::Y], sprite->get_where()[AXES::Z]));
		model = glm::rotate(model, sprite->get_rotation(), glm::vec3(0.0f, 0.0f, 1.0f));

		m_sprite_shader.load_uniform("model", model);
		//GLuint transformLoc = glGetUniformLocation(m_sprite_shader.get_adaptee_v(), "transform");
		//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		check_error(__FILE__, __LINE__);
	}
	else {
		tsg::logger::get_instance().write(tsg::string("{},{}: Error casting texture", __FILE__, __LINE__));
		assert(0); // texture is not a glfw_texture
	}
}

void glfw_renderer::draw(mesh const * const m) {
	/* TODO */
	assert(0);
}

void glfw_renderer::draw(font const * const f) {
	/* TODO */
	assert(0);
}

void glfw_renderer::draw(geometry::bounding_volume const * const bv) {
	switch (bv->get_type())
	{
	case geometry::bounding_volume::type::aabb:
	case geometry::bounding_volume::type::obb:
		if(bv->get_dimension() == 3) {
			draw(static_cast<geometry::box3D const * const>(bv));
		}
		else if(bv->get_dimension() == 2) {
			draw(static_cast<geometry::box2D const * const>(bv));
		}
		else {
			assert(0); // unsupported dimension
		}
		break;
	case geometry::bounding_volume::type::sphere:
		assert(0);
		break;
	default:
		break;
	}
}

/* TODO */
void glfw_renderer::draw(geometry::box3D const * const box) {
	assert(0);
}

void glfw_renderer::draw(geometry::box2D const * const box) {
	box_vertex<2> box2d_vertex(box, 0.9f, 0.8f, 0.7f, 1.0f);
	//glm::mat4 transform{ glm::mat4(1.0f) };  // make sure to initialize matrix to identity matrix first
	//float scale{ 0.5f };
	//transform = glm::scale(transform, glm::vec3(scale, scale, scale));
	//transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
	//transform = glm::rotate(transform, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	//GLint transformLoc = glGetUniformLocation(m_line_shader.get_adaptee_v(), "transform");
	//check_error(__FILE__, __LINE__);
	//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	//check_error(__FILE__, __LINE__);		
	glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	m_line_shader.load_uniform("model", model);
	box2d_vertex.init();
	box2d_vertex.use();
	box2d_vertex.draw();	
};

void glfw_renderer::draw(const drawable_bounding_volume&) {
	assert(0);
}

#endif