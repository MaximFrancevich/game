#include "uniquefilenamegenerator.h"
#include <filesystem>

const std::string UniqueFileNameGenerator::generate(const std::string& fileName, const std::string& extension) {
    auto result = fileName;
    for (auto i = 1; std::filesystem::exists(result + extension); i++) {
        result = fileName + std::to_string(i);
    }
    return result + extension;
}
