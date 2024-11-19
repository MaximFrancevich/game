#include "record.h"
#include <filesystem>
#include "uniquefilenamegenerator.h"
#include "mfile.h"
#include <QDebug>

Record::Record(const std::string& gameName, int finishedQuestsCount, int complitQuestsCount) :
    gameName { gameName },
    finishedQuestsCount { finishedQuestsCount },
    complitQuestsCount { complitQuestsCount }
{

}

void Record::save() const {
    std::filesystem::create_directory("records");
    auto recFileName = UniqueFileNameGenerator::generate("records/" + gameName, ".rec");
    auto recordFile = fopen(recFileName.c_str(), "wb");
    MFile::setCurrentFile(recordFile);
    if (recordFile) {
        MFile::write(gameName);
        MFile::write(finishedQuestsCount);
        MFile::write(complitQuestsCount);
    } else {
        qDebug() << "Ошибка сохранения рекорда";
    }
    fclose(recordFile);
}

std::unique_ptr<std::vector<Record>> Record::loadAllRecords(std::vector<std::string>& filePaths) {
    auto result = std::make_unique<std::vector<Record>>();
    for (auto& filePath : filePaths) {
        auto recordFile = fopen(filePath.c_str(), "rb");
        MFile::setCurrentFile(recordFile);
        auto name = MFile::readString();
        auto finishedQuestsCount = MFile::readInt();
        auto complitQuestsCount = MFile::readInt();
        result->push_back(Record(name, finishedQuestsCount, complitQuestsCount));
        fclose(recordFile);
    }
    return result;
}

const std::string& Record::getGameName() const {
    return gameName;
}

int Record::getFinishedQuestsCount() const {
    return finishedQuestsCount;
}

int Record::getComplitQuestsCount() const {
    return complitQuestsCount;
}
