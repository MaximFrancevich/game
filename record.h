#ifndef RECORD_H
#define RECORD_H

#include <string>
#include <memory>
#include <vector>

class Record
{
public:
    Record(const std::string& gameName, int finishedQuestsCount, int complitQuestsCount);
    void save() const;
    static std::unique_ptr<std::vector<Record>> loadAllRecords(std::vector<std::string>& filePaths);
    const std::string& getGameName() const;
    int getFinishedQuestsCount() const;
    int getComplitQuestsCount() const;

private:
    const std::string gameName;
    const int finishedQuestsCount;
    const int complitQuestsCount;
};

#endif // RECORD_H
