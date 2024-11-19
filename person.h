#ifndef PERSON_H
#define PERSON_H

#include "constants.h"
#include "characteristics/baseattribute.h"

#include <QMap>
#include "allattributes.h"
#include "characteristics/skills/skill.h"

class Person
{
public:
    Person(int trainingSpeed, int level = 0);
    Person();

    int getHp() const;
    int getAttributePointsCount() const;
    int hit(const Person &enemy, bool isCrit);
    void addAttributePoint();
    const std::array<std::unique_ptr<Attribute>, ATTRIBUTES_COUNT> &getAttributes();
    bool removeAttributePoint(int pointsCount = 1);
    void setHpAndManaToMax();
    const QString& attack(Person* enemy);
    const QString& getLog() const;
    void setLog(const QString &log);
    void addSkill(Skill* skill);
    std::vector<Skill*>& getSkills();
    void addSkillPoint();
    bool removeSkillPoints(int skillPointsCount = 1);
    int &getSkillPointsCount();
    Attribute* getAttributeWithMaxUpProfitIndex();
    int getFullAttributeValue(AttributeIndex attributeIndex) const;
    int getFullSkillsImpact(AttributeIndex attributeIndex, bool withCurrentActiveSkill) const;
    Skill* getRandomSkill();
    bool getCharacteristicCanUp(const Characteristic* characteristic);
    void upCharacteristic(Characteristic* characteristic);
    void setName(const QString& name);
    const QString& getName() const;
    void save() const;
    static void load(Person &person);
    void kill();
    bool getIsAlive() const;
    const std::map<int, const Skill*>& getActiveSkills() const;
    void setCurrentActiveSkillId(int id);
    int getCurrentActiveSkillId() const;
    int getMana() const;
    bool getActiveSkillCanUse(const Skill* skill);
    void refreshHpAndMana();

private:
    int hp;
    int attributePoints = cns::startSkillPoints;
    std::array<std::unique_ptr<Attribute>, ATTRIBUTES_COUNT> attributes;
    std::vector<Skill*> skills;
    int skillPoints = 0;
    QString name = "Персонаж";
    QString log;
    bool isAlive = true;
    std::map<int, const Skill*> activeSkills;
    int currentActiveSkillId = -1;
    int trainingSpeed;
    int mana;

    static int getFullCritDamage(const Person &attackingPerson);
    static int getFinalDamage(int damage, const Person& attackedPerson);
    static double getExpectedDamage(const Person &attackingPerson, const Person &attackedPerson);
    static double getExpectedTurnsCount(const Person &attackingPerson, const Person &attackedPerson);
    double getUpProfit(Characteristic *characteristic);
    static double getLargerQuotient(double firstValue, double secondValue);
};

#endif // PERSON_H
