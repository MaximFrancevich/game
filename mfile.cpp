#include "mfile.h"

static FILE* currentFile;

void MFile::setCurrentFile(FILE *file) {
    currentFile = file;
}

void MFile::write(const std::string &string) {
    int stringLength = string.length() * 2;
    write(stringLength);
    fwrite(string.c_str(), sizeof(char), stringLength, currentFile);
}

void MFile::write(int value) {
    fwrite(&value, sizeof(int), 1, currentFile);
}

const std::string MFile::readString() {
    int stringLength = readInt();
    char string[stringLength + 1];
    fread(&string, sizeof(char), stringLength, currentFile);
    string[stringLength] = '\0';
    return string;
}

int MFile::readInt() {
    int result;
    fread(&result, sizeof(int), 1, currentFile);
    return result;
}
