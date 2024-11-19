#ifndef GAME_H
#define GAME_H

#include "person.h"
#include "quest.h"

class Game
{
public:
    Game(int difficultyLevel, const std::string& name, int trainingSpeed);
    Game();
    std::map<int, Person>& getPersons();
    const std::vector<Person>* getEnemys() const;
    void clearPersonsLog();
    void nextFight();
    void startQuest(int index);
    void setPersonName(int personIndex, const QString &name);
    void finishQuest();
    const std::array<std::unique_ptr<Quest>, cns::questsCount>& getQuests() const;
    void setDifficultyLevel(int level);
    Person& getEnemy(int index);
    int getCurrentFightNumber() const;
    static void load(Game& game, const std::string& saveFileName);
    void save() const;
    const std::string& getName() const;
    const std::string& getSaveFileName() const;
    bool checkAlives();
    void complitQuest();
    std::unique_ptr<std::string> attackEnemy(int attackingPersonId, int attackedPersonId);
    int getDeadEnemiesCount() const;

private:
    std::map<int, Person> persons;
    std::unique_ptr<std::vector<Person>> enemies;
    std::array<std::unique_ptr<Quest>, cns::questsCount> quests;
    int currentQuestIndex;
    int finishedQuestsCount = 0;
    int difficultyLevel;
    int currentFightNumber;
    std::string name;
    std::string saveFileName;
    int complitQuestsCount = 0;
    int trainingSpeed;
    int deadEnemiesCount;

    void generateNewQuests();
};

#endif // GAME_H
