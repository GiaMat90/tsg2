#ifdef GLFW_GAME 

#include "glfw_font.h"
// engine includes
#include "gl_includes.h"
// std includes
#include <unordered_map>
#include <cassert>
// tsg includes
#include <io.h>
// freetype includes
#include <ft2build.h>
#include FT_FREETYPE_H

class character {
	friend glfw_font;
public:
	character() = default;
	~character() = default;
	character(const unsigned int id, std::size_t advance, const std::size_t size_x, const std::size_t size_y, const std::size_t bearing_x, const std::size_t bearing_y) :
		m_texture_id(id), m_advance(advance) {
		m_size[0] = size_x;
		m_size[1] = size_y;
		m_bearing[0] = bearing_x;
		m_bearing[1] = bearing_y;
	}
private:
	std::size_t m_size[2];			// Size of glyph
	std::size_t m_bearing[2];		// Offset from baseline to left/top of glyph		
	std::size_t m_advance{};		// Offset to advance to next glyph
	unsigned int m_texture_id{};	// ID handle of the glyph texture
};

static std::unordered_map<char, character> s_characters;

font* font::create_font() {
	return new glfw_font();
}

glfw_font::glfw_font() : font() {}

glfw_font::glfw_font(const std::string& file_name, const color& c, const texture::position& p, const std::size_t dim) : font(file_name, c, p) {
	load_font(file_name, dim);
}

glfw_font::~glfw_font() {
	cleanup();
}
void glfw_font::load_font(const std::size_t size, const font_type ft, const font_style fs) {
	std::filesystem::path file_path{};
	switch (ft) 
	{
	case font_type::arial:
		assert(0);
		switch (fs)
		{
		case font_style::bold:
			file_path = tsg::os::get_exe_path() / "fonts" / "arial_bold.ttf";
			break;
		case font_style::regular:
			file_path = tsg::os::get_exe_path() / "fonts" / "arial_regular.ttf";
			break;
		case font_style::italic:
			file_path = tsg::os::get_exe_path() / "fonts" / "arial_italic.ttf";
			break;
		default:
			break;
		}
		break;
	case font_type::carlito:
		switch (fs)
		{
		case font_style::bold:
			file_path = tsg::os::get_exe_path() / "fonts" / "carlito_bold.ttf";
			break;
		case font_style::regular:
			file_path = tsg::os::get_exe_path() / "fonts" / "carlito_regular.ttf";
			break;
		case font_style::italic:
			file_path = tsg::os::get_exe_path() / "fonts" / "carlito_italic.ttf";
			break;
		default:
			break;
		}
		break;
	case font_type::courier_new:
		assert(0);
		switch (fs)
		{
		case font_style::bold:
			file_path = tsg::os::get_exe_path() / "fonts" / "courier_new_bold.ttf";
			break;
		case font_style::regular:
			file_path = tsg::os::get_exe_path() / "fonts" / "courier_new_regular.ttf";
			break;
		case font_style::italic:
			file_path = tsg::os::get_exe_path() / "fonts" / "courier_new_italic.ttf";
			break;
		default:
			break;
		}
		break;
	case font_type::times_new_roman:
		assert(0);
		switch (fs)
		{
		case font_style::bold:
			file_path = tsg::os::get_exe_path() / "fonts" / "times_new_roman_bold.ttf";
			break;
		case font_style::regular:
			file_path = tsg::os::get_exe_path() / "fonts" / "times_new_roman_regular.ttf";
			break;
		case font_style::italic:
			file_path = tsg::os::get_exe_path() / "fonts" / "times_new_roman_italic.ttf";
			break;
		default:
			break;
		}
		break;
	case font_type::verdana:
		assert(0);
		switch (fs)
		{
		case font_style::bold:
			file_path = tsg::os::get_exe_path() / "fonts" / "verana_bold.ttf";
			break;
		case font_style::regular:
			file_path = tsg::os::get_exe_path() / "fonts" / "verana_regular.ttf";
			break;
		case font_style::italic:
			file_path = tsg::os::get_exe_path() / "fonts" / "verana_italic.ttf";
			break;
		default:
			break;
		}
		break;
	default:
		tsg::print("Unknown font type");
		assert(0);
		break;
	}
	load_font(file_path, size);
}
void glfw_font::load_font(const std::filesystem::path& file_name, const std::size_t dim) {
	assert(false == file_name.empty());
	if (false == s_characters.empty()) {
		cleanup();
	}
	FT_Library ft;
	if(FT_Error err = FT_Init_FreeType(&ft)) {
		tsg::print("Could not init FreeType Library, FT error {}", err);
		assert(0);
	}
	// Load font as face
	FT_Face face;
	if(FT_Error err = FT_New_Face(ft, file_name.string().c_str(), 0, &face)) {
		tsg::print("Failed to load font: {}, FT error {}", file_name, err);
		assert(0);
	}
	// Set size to load glyphs
	FT_Set_Pixel_Sizes(face, 0, static_cast<FT_UInt>(dim));
	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
	// Load first 128 characters of ASCII set
	for (unsigned char c{ 0 }; c < 128; ++c) {
		// Load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			tsg::print("Failed to load Glyph: {}", c);
			continue;
		}
		// Generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		character character_instance(
			texture,
			static_cast<std::size_t>(face->glyph->advance.x),
			static_cast<std::size_t>(face->glyph->bitmap.width),
			static_cast<std::size_t>(face->glyph->bitmap.rows),
			static_cast<std::size_t>(face->glyph->bitmap_left),
			static_cast<std::size_t>(face->glyph->bitmap_top)
		);
		s_characters.insert(std::pair<char, character>(c, character_instance));
	}
	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
	// Cleanup FreeType resources
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void glfw_font::compute_text_size(const std::string& s, const float scale) {
	assert(s.empty());
	size size{ .width{0u}, .height{0u} };
	for (const char& c : s) {
		const character& ch = s_characters.at(c);
		size.width += static_cast<std::size_t>((ch.m_advance >> 6) * scale); // bitshift by 6 to get value in pixels (2^6 = 64)
		const std::size_t h = static_cast<std::size_t>(ch.m_size[1] * scale);
		if (h > size.height) {
			size.height = h;
		}
	}
	m_width = static_cast<int>(size.width);
	m_height = static_cast<int>(size.height);
}
glfw_font::size glfw_font::get_text_size(const std::string& s, const float scale) const {
	assert(s.empty());
	size size{ .width{0u}, .height{0u} };
	for (const char& c : s) {
		const character& ch = s_characters.at(c);
		size.width += static_cast<std::size_t>((ch.m_advance >> 6) * scale); // bitshift by 6 to get value in pixels (2^6 = 64)
		const std::size_t h = static_cast<std::size_t>(ch.m_size[1] * scale);
		if (h > size.height) {
			size.height = h;
		}
	}
	return size;
}

glfw_font::size glfw_font::get_char_size(const char c) const {
	const character& ch = s_characters.at(c);
	size size{
		.width{ static_cast<std::size_t>(ch.m_size[0] * m_scale) },
		.height{ static_cast<std::size_t>(ch.m_size[1] * m_scale) }
	};
	return size;
}

glfw_font::pos glfw_font::get_char_pos(const char c) const {
	const character& ch = s_characters.at(c);
	pos position{
		.x{ static_cast<std::size_t>( m_position[0] + ch.m_bearing[0]*m_scale) },
		.y{ static_cast<std::size_t>(m_position[1] - (ch.m_size[1] - ch.m_bearing[1]) * m_scale) }
	};
	return position;
}

void glfw_font::cleanup() {
	for(auto& pair : s_characters) {
		glDeleteTextures(1, &pair.second.m_texture_id);
	}
	s_characters.clear();
}

void glfw_font::set_active() const {
	// with one should I do?
	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, m_raw_attribute);
}

void glfw_font::bind_char_texture(const char c) const {
	const character& ch = s_characters.at(c);
	glBindTexture(GL_TEXTURE_2D, ch.m_texture_id);
}

#else
static_assert(false, "GLFW_GAME is not defined");
#endif