#include "game.h"
#include "enemysgenerator.h"
#include "fstream"
#include <QDebug>
#include <filesystem>
#include "uniquefilenamegenerator.h"
#include "mfile.h"
#include "record.h"

Game::Game(int difficultyLevel, const std::string& name, int trainingSpeed) :
    difficultyLevel { difficultyLevel },
    name { name },
    trainingSpeed { trainingSpeed }
{
    persons[0] = Person(trainingSpeed, cns::playerStartLevel);
    generateNewQuests();
    std::filesystem::create_directory("saves");
    saveFileName = UniqueFileNameGenerator::generate("saves/" + name, ".sav");
}

void Game::complitQuest() {
    complitQuestsCount++;
}

std::unique_ptr<std::string> Game::attackEnemy(int attackingPersonId, int attackedPersonId)
{
    auto result = std::make_unique<std::string>(persons[attackingPersonId].attack(&enemies->at(attackedPersonId)).toStdString());
    if (enemies->at(attackedPersonId).getHp() <= 0) {
        deadEnemiesCount++;
    }
    return result;
}

int Game::getDeadEnemiesCount() const
{
    return deadEnemiesCount;
}

Game::Game() {

}

std::map<int, Person>& Game::getPersons() {
    return persons;
}

const std::vector<Person>* Game::getEnemys() const {
    return enemies.get();
}

void Game::clearPersonsLog() {
    for (auto& person : persons) {
        person.second.setLog("");
    }
    for (auto& enemy : *enemies) {
        enemy.setLog("");
    }
}

void Game::nextFight() {
    deadEnemiesCount = 0;
    currentFightNumber++;
    enemies = EnemiesGenerator::generateEnemyByLevel(quests[currentQuestIndex]->getDifficultyLevel());
}

void Game::startQuest(int index) {
    currentQuestIndex = index;
    for (auto& person : persons) {
        person.second.setHpAndManaToMax();
    }
}

void Game::setPersonName(int personIndex, const QString &name) {
    persons[personIndex].setName(name);
}

void Game::generateNewQuests() {
    currentFightNumber = 0;
    for (int i = 0; i < cns::questsCount; i++) {
        quests[i] = std::make_unique<Quest>(finishedQuestsCount * difficultyLevel / 5 + i);
    }
}

void Game::finishQuest() {
    finishedQuestsCount++;
    std::vector<int> deadPersonsIds;
    for (auto &person : persons) {
        if (!person.second.getIsAlive()) {
            deadPersonsIds.push_back(person.first);
        }
    }
    for (auto& id : deadPersonsIds) {
        persons.erase(id);
    }
    generateNewQuests();
}

const std::string& Game::getName() const {
    return name;
}

const std::string& Game::getSaveFileName() const {
    return saveFileName;
}

const std::array<std::unique_ptr<Quest>, cns::questsCount>& Game::getQuests() const {
    return quests;
}

void Game::setDifficultyLevel(int level) {
    difficultyLevel = level;
}

Person& Game::getEnemy(int index) {
    return enemies->at(index);
}

int Game::getCurrentFightNumber() const {
    return currentFightNumber;
}

void Game::save() const {
    auto saveFile = fopen(saveFileName.c_str(), "wb");
    if (!saveFile) {
        qDebug() << "Ошибка сохранения";
        return;
    }
    MFile::setCurrentFile(saveFile);
    Skill::saveAllSkills();
    MFile::write(trainingSpeed);
    MFile::write(persons.size());
    for (auto &person : persons) {
        MFile::write(person.first);
        person.second.save();
    }
    MFile::write(finishedQuestsCount);
    MFile::write(difficultyLevel);
    MFile::write(name);
    MFile::write(complitQuestsCount);
    fclose(saveFile);
}

void Game::load(Game &game, const std::string& saveFileName) {
    auto saveFile = fopen(saveFileName.c_str(), "rb");
    if (!saveFile) {
        qDebug() << "Ошибка загрузки";
        return;
    }
    MFile::setCurrentFile(saveFile);
    Skill::loadAllSkills();
    game.trainingSpeed = MFile::readInt();
    int personsCount = MFile::readInt();
    game.persons.clear();
    for (auto i = 0; i < personsCount; i++) {
        int personId = MFile::readInt();
        game.persons[personId] = Person(game.trainingSpeed);
        Person::load(game.persons[personId]);
    }
    game.finishedQuestsCount = MFile::readInt();
    game.difficultyLevel = MFile::readInt();
    game.name = MFile::readString();
    game.complitQuestsCount = MFile::readInt();
    fclose(saveFile);
    game.saveFileName = saveFileName;
    game.generateNewQuests();
}

bool Game::checkAlives() {
    if (!persons.size()) {
        remove(saveFileName.c_str());
        Record(name, finishedQuestsCount, complitQuestsCount).save();
        return false;
    } else {
        return true;
    }
}
