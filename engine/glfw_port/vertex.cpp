#ifdef GLFW_GAME 

#include "vertex.h"
/* std includes */
#include <cassert>

vertex::vertex(const GLuint vertex, const GLuint index) : m_vertex_buffer(vertex), m_index_buffer(index) {}

/* sprite impl */
texture_vertex::texture_vertex() : vertex() {}

texture_vertex::~texture_vertex()
{
	glDeleteBuffers(1, &m_vertex_buffer);
	glDeleteBuffers(1, &m_index_buffer);
	glDeleteVertexArrays(1, &m_raw_attribute);
}


void texture_vertex::init() {
	// texture - buffers and arrays
	glGenVertexArrays(1, &m_raw_attribute);
	check_error(__FILE__, __LINE__);
	glBindVertexArray(m_raw_attribute);
	check_error(__FILE__, __LINE__);

	glGenBuffers(1, &m_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexes), m_vertexes, GL_STATIC_DRAW);
	check_error(__FILE__, __LINE__);

	glGenBuffers(1, &m_index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indexes), m_indexes, GL_STATIC_DRAW);
	check_error(__FILE__, __LINE__);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
	check_error(__FILE__, __LINE__);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, reinterpret_cast<void*>(sizeof(float) * 3));
	check_error(__FILE__, __LINE__);

	glBindVertexArray(0); // Unbind
}

void texture_vertex::use() {
	glBindVertexArray(m_raw_attribute);
	check_error(__FILE__, __LINE__);
}
void texture_vertex::draw() {
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	check_error(__FILE__, __LINE__);
}


/* TODO: mesh impl */
mesh_vertex::mesh_vertex() : vertex() {}
mesh_vertex::~mesh_vertex() {}
void mesh_vertex::init() {
	assert(0);
}
void mesh_vertex::use() {
	assert(0);
}
void mesh_vertex::draw() {
	assert(0);
}
/* box2D impl */
box2D_vertex::box2D_vertex() : vertex() {}

box2D_vertex::~box2D_vertex()
{
	glDeleteBuffers(1, &m_vertex_buffer);
	glDeleteBuffers(1, &m_index_buffer);
	glDeleteVertexArrays(1, &m_raw_attribute);
}

void box2D_vertex::init() {
	// texture - buffers and arrays
	glGenVertexArrays(1, &m_raw_attribute);
	check_error(__FILE__, __LINE__);
	glBindVertexArray(m_raw_attribute);
	check_error(__FILE__, __LINE__);
	glGenBuffers(1, &m_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexes), m_vertexes, GL_STATIC_DRAW);
	check_error(__FILE__, __LINE__);
	glGenBuffers(1, &m_index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_index_buffer), m_indexes, GL_STATIC_DRAW);
	check_error(__FILE__, __LINE__);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	check_error(__FILE__, __LINE__);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, reinterpret_cast<void*>(sizeof(float) * 3));
	check_error(__FILE__, __LINE__);
	// unbind all
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void box2D_vertex::use() {
	glBindVertexArray(m_raw_attribute);
	check_error(__FILE__, __LINE__);
}

void box2D_vertex::draw() {
	glBindVertexArray(m_raw_attribute);
	check_error(__FILE__, __LINE__);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	check_error(__FILE__, __LINE__);
}

line_vertex::line_vertex(const float r, const float b, const float g, const float a) : vertex() 
{
	// color first vertex
	m_vertexes[3] = r;
	m_vertexes[4] = b;
	m_vertexes[5] = g;
	m_vertexes[6] = a;
	// color second vertex
	m_vertexes[10] = r;
	m_vertexes[11] = b;
	m_vertexes[12] = g;
	m_vertexes[13] = a;
}
line_vertex::~line_vertex()
{
	glDeleteBuffers(1, &m_vertex_buffer);
	glDeleteVertexArrays(1, &m_raw_attribute);
}
void line_vertex::init() {
	// line - buffers and arrays
	glGenVertexArrays(1, &m_raw_attribute);
	check_error(__FILE__, __LINE__);
	glBindVertexArray(m_raw_attribute);
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

void line_vertex::use() {
	glBindVertexArray(m_raw_attribute);
	check_error(__FILE__, __LINE__);
}

void line_vertex::draw() {
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
	check_error(__FILE__, __LINE__);
}


#endif