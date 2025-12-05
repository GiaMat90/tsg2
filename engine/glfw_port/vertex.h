#pragma once

#include "gl_includes.h"
#include "gl_utility.h"
#include <geometry.h>
/* tsg includes */
#include <tsg/types.h>
#include <tsg/logger.h>

/* std includes */
#include <numbers>	// pi

class vertex : public tsg::adapter_v<GLuint> {
public:
	vertex(const GLuint vertex = 0u, const GLuint index = 0u);
	virtual ~vertex() = default;
	virtual void init() = 0;
	virtual void use() = 0;
	virtual void draw() = 0;
protected:
	GLuint m_vertex_buffer{};
	GLuint m_index_buffer{};
};

class texture_vertex : public vertex {
public:
	texture_vertex();
	~texture_vertex();
	void init() override;
	void use() override;
	void draw() override;
private:
	const float m_vertexes[20] = {
		// Vertex Positions // Texture Coords
		-0.5f,  0.5f, 0.f,	0.f, 0.f, // top left
		 0.5f,  0.5f, 0.f,	1.f, 0.f, // top right
		 0.5f, -0.5f, 0.f,	1.f, 1.f, // bottom right
		-0.5f, -0.5f, 0.f,	0.f, 1.f  // bottom left
	};
	const int m_indexes[6] = {
		0, 1, 2,
		2, 3, 0 
	};
};

class mesh_vertex : public vertex {
public:
	mesh_vertex();
	virtual ~mesh_vertex();
public:
	void init() override;
	void use() override;
	void draw() override;
};

class box2D_vertex : public vertex {
public:
	box2D_vertex();
	~box2D_vertex();
	void init() override;
	void use() override;
	void draw() override;
private:
	const float m_vertexes[20] = {
		-0.5f,  0.5f, 0.f, 0.f, 0.f, // top left
		 0.5f,  0.5f, 0.f, 1.f, 0.f, // top right
		 0.5f, -0.5f, 0.f, 1.f, 1.f, // bottom right
		-0.5f, -0.5f, 0.f, 0.f, 1.f  // bottom left
	};
	const int m_indexes[6] = {
		0, 1, 2,
		2, 3, 0
	};
};

class line_vertex : public vertex {
public:
	line_vertex(const float r = 1.0f, const float b = 1.0f, const float g = 1.0f, const float a = 1.0f);
	~line_vertex();
	void init() override;
	void use() override;
	void draw() override;
private:
	float m_vertexes[14] = {
		// Vertex Positions   // Color RGBA
		-0.5f,  0.0f, 0.0f,	  0.0f, 0.0f, 0.0f, 1.0f,	// start position
		 0.5f,  0.0f, 0.0f,	  0.0f, 0.0f, 0.0f, 1.0f	// end	position
	};
};

template <std::size_t N>
class regpoly_vertex : public vertex {
public:
	regpoly_vertex(const float r = 1.0f, const float b = 1.0f, const float g = 1.0f, const float a = 1.0f) {
		static_assert(N > 1);
		constexpr float delta_alpha{ ((360.0f / N) * std::numbers::pi) / 180.0f };
		constexpr float radius{ 1.0f };
		tsg::logger::get_instance().write(tsg::string("dalpha={}, radius={}", delta_alpha, radius));
		float alpha{ 0.0f };
		std::size_t i{ 0u };
		while (i < 7u * N) {
			// first vertex position
			m_vertexes[i++] = radius * cosf(alpha);
			m_vertexes[i++] = radius * sinf(alpha);
			m_vertexes[i++] = 0.0f;
			m_vertexes[i++] = r;
			m_vertexes[i++] = g;
			m_vertexes[i++] = b;
			m_vertexes[i++] = a;
			alpha += delta_alpha;
			tsg::logger::get_instance().write(tsg::string("Vertex {}: pos=({}, {}, {})", i / 7u - 1, m_vertexes[i - 7u], m_vertexes[i - 6u], m_vertexes[i - 5u]));
		}
	}
	~regpoly_vertex() {
		glDeleteBuffers(1, &m_vertex_buffer);
		glDeleteVertexArrays(1, &m_adaptee);
	}
	void init() override {
		// line - buffers and arrays
		glGenVertexArrays(1, &m_adaptee);
		check_error(__FILE__, __LINE__);
		glBindVertexArray(m_adaptee);
		check_error(__FILE__, __LINE__);
		glGenBuffers(1, &m_vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexes), m_vertexes, GL_STATIC_DRAW);
		check_error(__FILE__, __LINE__);
		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, reinterpret_cast<void*>(0));
		glEnableVertexAttribArray(0);
		check_error(__FILE__, __LINE__);
		// color attribute
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 7, reinterpret_cast<void*>(sizeof(float) * 3));
		glEnableVertexAttribArray(1);
		check_error(__FILE__, __LINE__);
		// unbind all
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void use() override {
		glBindVertexArray(m_adaptee);
		check_error(__FILE__, __LINE__);
	}
	void draw() override {
		glDrawArrays(GL_LINE_LOOP, 0, N);
		glBindVertexArray(0);
		check_error(__FILE__, __LINE__);
	}
private:
	float m_vertexes[7u * N];
};

template <std::size_t Dim> requires geometry::GeometricDimension<Dim>
class box_vertex : public vertex {
public:
	box_vertex(geometry::box<Dim> * const box, const float r = 1.0f, const float b = 1.0f, const float g = 1.0f, const float a = 1.0f) : m_box(box) {
		std::size_t i{ 0u };
		std::size_t j{ 0u };
		while (i < 7u * Dim * Dim) {
			// first vertex position
			m_vertexes[i++] = m_box->get_edges().at(j).get_start()[0]; //m_box->get_vertexes().at(j)[0];
			m_vertexes[i++] = m_box->get_edges().at(j).get_start()[1]; //m_box->get_vertexes().at(j)[1];
			if constexpr (Dim == 3) {
				m_vertexes[i++] = m_box->get_edges().at(j).get_start()[2]; //m_box->get_vertexes().at(j)[2];
			}
			else {
				m_vertexes[i++] = 0.0f;
			}
			m_vertexes[i++] = r;
			m_vertexes[i++] = g;
			m_vertexes[i++] = b;
			m_vertexes[i++] = a;
			++j;
			//tsg::logger::get_instance().write(tsg::string("Box{}DVertex {}: pos=({}, {}, {})", Dim, i / 7 - 1, m_vertexes[i - 7], m_vertexes[i - 6], m_vertexes[i - 5]));
		}
	}
	~box_vertex() {
		glDeleteBuffers(1, &m_vertex_buffer);
		glDeleteVertexArrays(1, &m_adaptee);
	}
	void init() override {
		// line - buffers and arrays
		glGenVertexArrays(1, &m_adaptee);
		check_error(__FILE__, __LINE__);
		glBindVertexArray(m_adaptee);
		check_error(__FILE__, __LINE__);
		glGenBuffers(1, &m_vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexes), m_vertexes, GL_STATIC_DRAW);
		check_error(__FILE__, __LINE__);
		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, reinterpret_cast<void*>(0));
		glEnableVertexAttribArray(0);
		check_error(__FILE__, __LINE__);
		// color attribute
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 7, reinterpret_cast<void*>(sizeof(float) * 3));
		glEnableVertexAttribArray(1);
		check_error(__FILE__, __LINE__);
		// unbind all
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void use() override {
		glBindVertexArray(m_adaptee);
		check_error(__FILE__, __LINE__);
	}
	void draw() override {
		glDrawArrays(GL_LINE_LOOP, 0, Dim * Dim);
		glBindVertexArray(0);
		check_error(__FILE__, __LINE__);
	}
private:
	// N dimensional Box has N^2 vertexes, each one with 3 position coords and 4 color RGBA
	float m_vertexes[7u * Dim * Dim];
	geometry::box<Dim>* m_box;
};
