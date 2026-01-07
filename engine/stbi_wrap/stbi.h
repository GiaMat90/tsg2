#pragma once

#include <string>

class stbi {
public:
	struct data {
		unsigned char* m_data = nullptr;
		int m_width = 0;
		int m_height = 0;
		int m_channels = 0;
	};
public:
	stbi(const std::string& image);
	~stbi();
public:
	bool load();
	inline data const * const get_data() const { return &m_data; };
	const char* get_failure_reason() const;
public:
	std::string m_image;
	data m_data;
};