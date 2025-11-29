#pragma once

#include "gl_includes.h"

/* tsg includes */
#include <tsg/types.h>

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
		// Vertex Positions // Color RGBA
		-0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,	// start position
		 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f	// end	position
	};
};