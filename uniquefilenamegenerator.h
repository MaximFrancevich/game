#ifndef UNIQUEFILENAMEGENERATOR_H
#define UNIQUEFILENAMEGENERATOR_H

#include <string>

class UniqueFileNameGenerator
{
public:
    static const std::string generate(const std::string& fileName, const std::string& extension);
};

#endif // UNIQUEFILENAMEGENERATOR_H
