#pragma once
#include <fstream>
#include <vector>

namespace Utils
{
	bool read_file(const std::string& filePath, std::vector<uint32_t>& buffer);
}