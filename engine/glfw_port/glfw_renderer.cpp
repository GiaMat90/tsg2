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

using geometry::axes;

glfw_renderer::glfw_renderer(glfw_window * const w, glfw_camera * const c) : renderer<glfw_window, glfw_renderer, glfw_camera>(w, c) {
	// NOTE: OpenGL error checks have been omitted for brevity
	if (0 == gladLoadGL(glfwGetProcAddress)) {
		throw exception("gladLoadGL Error");
	};
	glfwSwapInterval(1);

	m_texture_shader.init(
		(tsg::os::get_exe_path() / std::filesystem::path("shaders") / std::filesystem::path("texture_vert.glsl")).string().c_str(),
		(tsg::os::get_exe_path() / std::filesystem::path("shaders") / std::filesystem::path("texture_frag.glsl")).string().c_str()
	);
	m_texture_vertex.init();
	
	m_sprite_shader.init(
		(tsg::os::get_exe_path() / std::filesystem::path("shaders") / std::filesystem::path("texture_vert.glsl")).string().c_str(),
		(tsg::os::get_exe_path() / std::filesystem::path("shaders") / std::filesystem::path("texture_frag.glsl")).string().c_str()
	);
	m_sprite_vertex.init();

	m_line_shader.init(
		(tsg::os::get_exe_path() / std::filesystem::path("shaders") / std::filesystem::path("line_vert.glsl")).string().c_str(),
		(tsg::os::get_exe_path() / std::filesystem::path("shaders") / std::filesystem::path("line_frag.glsl")).string().c_str()
	);
	m_line_vertex.init();

	m_font_shader.init(
		(tsg::os::get_exe_path() / std::filesystem::path("shaders") / std::filesystem::path("font_vert.glsl")).string().c_str(),
		(tsg::os::get_exe_path() / std::filesystem::path("shaders") / std::filesystem::path("font_frag.glsl")).string().c_str()
	);
	m_font_vertex.init();
}

glfw_renderer::~glfw_renderer() { /* Nothing to do */ }

// overrided methods
void glfw_renderer::render() {
	/* Render here */
	check_error(__FILE__, __LINE__);
	int width, height;
	glfwGetFramebufferSize(m_window->cget_raw_attribute(), &width, &height);
	const float ratio = static_cast<float>(width) / static_cast<float>(height);

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
		m_line_shader.load_uniform("view", m_camera->get_view());
		m_line_shader.load_uniform("projection", m_camera->get_projection());
		for (const auto& bv : m_bv_obj) {
			this->draw(bv);
		}
		m_line_shader.unuse();
	}
	/* drawing fonts */ 
	if (m_fonts_obj.size() > 0) {
		// use programs previously loaded
		m_font_shader.use();
		m_font_shader.load_uniform("projection",
			//glm::ortho(0.0f, static_cast<float>(m_window->get_width()), 0.0f, static_cast<float>(m_window->get_height())));
			//glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f)
			m_camera->get_projection()
		);
		m_font_vertex.use();
		for (const auto& f : m_fonts_obj) {
			this->draw(f);
		}
		m_font_shader.unuse();
	}
	/* Swap front and back buffers */
	glfwSwapBuffers(m_window->cget_raw_attribute());
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
		assert(glIsTexture(texture->cget_raw_attribute())); // chack if texture is not a valid OpenGL texture

		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		model = glm::translate(model, glm::vec3(texture->get_where()[axes::x], texture->get_where()[axes::y], texture->get_where()[axes::z]));
		model = glm::rotate(model, texture->get_rotation(), glm::vec3(0.0f, 0.0f, 1.0f));

		unsigned int transformLoc = glGetUniformLocation(m_texture_shader.cget_raw_attribute(), "model");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));
		check_error(__FILE__, __LINE__);

		glBindTexture(GL_TEXTURE_2D, texture->cget_raw_attribute());
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
		assert(glIsTexture(sprite->cget_raw_attribute()));

		sprite->set_active();

		GLint boundTexture;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexture);
		assert(boundTexture == sprite->cget_raw_attribute());

		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		model = glm::translate(model, glm::vec3(sprite->get_where()[axes::x], sprite->get_where()[axes::y], sprite->get_where()[axes::z]));
		model = glm::rotate(model, sprite->get_rotation(), glm::vec3(0.0f, 0.0f, 1.0f));

		m_sprite_shader.load_uniform("model", model);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		check_error(__FILE__, __LINE__);
	}
	else {
		tsg::logger::get_instance().write(tsg::string("{},{}: Error casting texture", __FILE__, __LINE__));
		assert(0); // texture is not a glfw_texture
	}
}

void glfw_renderer::draw(mesh * const m) {
	/* TODO */
	assert(0);
}

void glfw_renderer::draw(font * const f) {
	if (auto font_impl = dynamic_cast<glfw_font*>(f)) {
		m_font_shader.load_uniform("textColor",
			//glm::vec3(font_impl->get_color().get_normalized(color::red), font_impl->get_color().get_normalized(color::green), font_impl->get_color().get_normalized(color::blue))
			glm::vec3(0.9f, 0.8f, 0.7f) // inverse of background color petrolio
		);
		font_impl->set_active();
		const std::string text = font_impl->get_text();
		for (const auto c : text)
		{			
			auto pos{ font_impl->get_char_pos(c) };
			auto size{ font_impl->get_char_size(c) };
			//font_vertex fvert(
			//	static_cast<float>(pos.x) / static_cast<float>(m_window->get_width()),
			//	static_cast<float>(pos.y) / static_cast<float>(m_window->get_height()),
			//	static_cast<float>(size.width) / static_cast<float>(m_window->get_width()),
			//	static_cast<float>(size.height) / static_cast<float>(m_window->get_height())
			//);
			//fvert.init();
			//fvert.use();
			//font_impl->bind_char_texture(c);
			//fvert.draw();
			////fvert.unuse();
			/* Other way */
			font_impl->bind_char_texture(c);
			m_font_vertex.draw(
				static_cast<float>(pos.x) / static_cast<float>(m_window->get_width()),
				static_cast<float>(pos.y) / static_cast<float>(m_window->get_height()),
				static_cast<float>(size.width) / static_cast<float>(m_window->get_width()),
				static_cast<float>(size.height) / static_cast<float>(m_window->get_height())
			);
			check_error(__FILE__, __LINE__);
		}
	}
	else {
		tsg::logger::get_instance().write(tsg::string("{},{}: Error casting texture", __FILE__, __LINE__));
		assert(0); // texture is not a glfw_texture
	}
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
	glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	m_line_shader.load_uniform("model", model);
	box_vertex<2> box2d_vertex(box, 0.9f, 0.8f, 0.7f, 1.0f);
	box2d_vertex.init();
	box2d_vertex.use();
	box2d_vertex.draw();	
};

void glfw_renderer::draw(const drawable_bounding_volume&) {
	assert(0);
}

#else
static_assert(false, "GLFW_GAME is not defined");
#endif