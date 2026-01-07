#include "stbi.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>      // stbi_load

stbi::stbi(const std::string& image)
	: m_image(image) {
}

stbi::~stbi() {
	if (m_data.m_data) {
		stbi_image_free(m_data.m_data);
		m_data.m_data = nullptr;
	}
}

bool stbi::load() {
	m_data.m_data = stbi_load(m_image.c_str(), &m_data.m_width, &m_data.m_height, &m_data.m_channels, 0);
	return m_data.m_data != nullptr;
}

const char* stbi::get_failure_reason() const {
	return stbi_failure_reason();
}