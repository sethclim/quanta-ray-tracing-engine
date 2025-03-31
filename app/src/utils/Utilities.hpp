#ifndef UTILITIES_H
#define UTILITIES_H


#include <fstream>
#include <vector>


namespace Utils
{
	bool read_file(const std::string& filePath, std::vector<uint32_t>& buffer);
}

#endif