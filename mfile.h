#ifndef MFILE_H
#define MFILE_H

#include <string>

class MFile
{
public:
    static void setCurrentFile(FILE* file);
    static void write(const std::string& string);
    static void write(int value);
    static const std::string readString();
    static int readInt();
};

#endif // MFILE_H
