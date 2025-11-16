#pragma once

#include <string>

class asset {
public:
	asset(const std::string& file_path = "") : m_file_path(file_path) {};
	~asset() = default;
	inline void set_path(const std::string& path) { m_file_path = path;}
	inline std::string get_path() { return m_file_path; }
private:
	std::string m_file_path{};
};