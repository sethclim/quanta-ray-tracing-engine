
#include "Utilities.hpp"


namespace Utils
{
	bool read_file(const std::string &filePath, std::vector<uint32_t> &buffer)
    {
        // open the file. With cursor at the end
        std::ifstream file(filePath, std::ios::ate | std::ios::binary);

        if (!file.is_open())
        {
            return false;
        }

        // find what the size of the file is by looking up the location of the cursor
        // because the cursor is at the end, it gives the size directly in bytes
        size_t fileSize = (size_t)file.tellg();

        // // spirv expects the buffer to be on uint32, so make sure to reserve a int
        // // vector big enough for the entire file
        // buffer(fileSize / sizeof(uint32_t));
        buffer.resize(fileSize / sizeof(uint32_t));

        // put file cursor at beginning
        file.seekg(0);

        // load the entire file into the buffer
        file.read((char *)buffer.data(), fileSize);

        // now that the file is loaded into the buffer, we can close it
        file.close();

        return true;
    }
}