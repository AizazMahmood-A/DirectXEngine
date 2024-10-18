#include <string>
namespace Utils {
    bool static hasExtension(const std::string& filename, const std::string& extension) {
        // Find the last dot in the filename
        size_t dotPosition = filename.find_last_of('.');

        // If there's no dot or it's the first character (hidden files in Unix-like systems), return false
        if (dotPosition == std::string::npos || dotPosition == 0) {
            return false;
        }

        std::string fileExtension = filename.substr(dotPosition + 1);
        return fileExtension == extension;
    }

    bool static hasExtension(const char* filename, const char* extension) {
        // Find the last dot in the filename using strrchr (returns pointer to the last occurrence of '.')
        const char* dotPosition = strrchr(filename, '.');

        // If there's no dot or if it's the first character, return false
        if (dotPosition == nullptr || dotPosition == filename) {
            return false;
        }

        return strcmp(dotPosition + 1, extension) == 0;
    }
}