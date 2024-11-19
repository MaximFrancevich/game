#include "person.h"
#include "allattributes.h"
#include "bonus.h"
#include <QDebug>
#include "fstream"
#include "mfile.h"

void Person::setName(const QString &name) {
    this->name = name;
}

const QString& Person::getName() const {
    return name;
}

Person::Person(int trainingSpeed, int level) : trainingSpeed { trainingSpeed }
{
    for (unsigned i = 0; i < AllAttributes::getAttributes()->size(); i++) {
        attributes[i] = std::make_unique<Attribute>(AllAttributes::getAttributes()->at(i).get());
        attributes[i]->setValue(attributes[i]->getStaticAttribute()->getMinValue() +
                                attributes[i]->getStaticAttribute()->getUpValue() * level);
    }
    setHpAndManaToMax();
}

Person::Person()
{

}

int Person::getHp() const {
    return hp;
}

bool Person::getCharacteristicCanUp(const Characteristic* characteristic) {
    auto skill = dynamic_cast<const Skill*>(characteristic);
    if (skill) {
        if (skill->getParent() && skill->getValue() == skill->getParent()->getValue()) {
            return false;
        }
        if (skill->getType() == ACTIVE) {
            return true;
        }
        for (unsigned i = 0; i < skill->getAttributeImpacts().size(); i++) {
            if (getFullAttributeValue((AttributeIndex)i) + skill->getAttributeImpacts().at(i) * AllAttributes::getAttributes()->at(i)->getUpValue() <
                    AllAttributes::getAttributes()->at(i)->getMinValue()) {
                return false;
            }
        }
    }
    return true;
}

int Person::hit(const Person &enemy, bool isCrit) {
    auto damage = enemy.getFullAttributeValue(DAMAGE);
    if (isCrit) {
        damage += enemy.getFullAttributeValue(CRIT_DAMAGE);
    }
    damage = getFinalDamage(damage, *this);
    hp -= damage;
    if (hp <= 0) {
        hp = 0;
        log.append(*Bonus::bonus(*this, false));
    }
    return damage;
}

void Person::addAttributePoint() {
    attributePoints++;
}

int Person::getAttributePointsCount() const {
    return attributePoints;
}

const std::array<std::unique_ptr<Attribute>, ATTRIBUTES_COUNT> &Person::getAttributes() {
    return attributes;
}

bool Person::removeAttributePoint(int pointsCount) {
    if (pointsCount <= attributePoints) {
        attributePoints -= pointsCount;
        return true;
    } else {
        return false;
    }
}

void Person::setHpAndManaToMax() {
    hp = getFullAttributeValue(MAX_HP);
    mana = getFullAttributeValue(MANA);
}

int Person::getFullCritDamage(const Person &attackingPerson) {
    return attackingPerson.getFullAttributeValue(DAMAGE) + attackingPerson.getFullAttributeValue(CRIT_DAMAGE);
}

int Person::getFinalDamage(int damage, const Person &attackedPerson) {
    int finalDamage = damage - attackedPerson.getFullAttributeValue(ARMOR);
    if (finalDamage < 1) {
        finalDamage = 1;
    }
    int resistanceSave = finalDamage * attackedPerson.getFullAttributeValue(RESISTANCE) / 100;
    return finalDamage - resistanceSave;
}

double Person::getExpectedDamage(const Person &attackingPerson, const Person &attackedPerson) {
    return attackingPerson.getFullAttributeValue(CRIT_CHANCE) / 100.0 * getFinalDamage(getFullCritDamage(attackingPerson), attackedPerson) +
            (100 - attackingPerson.getFullAttributeValue(CRIT_CHANCE)) /
            100.0 *
            getFinalDamage(attackingPerson.getFullAttributeValue(DAMAGE), attackedPerson);
}

double Person::getExpectedTurnsCount(const Person &attackingPerson, const Person &attackedPerson) {
    int expectedDamage = getExpectedDamage(attackingPerson, attackedPerson);
    return (double)attackedPerson.getFullAttributeValue(MAX_HP) /
            attackingPerson.getFullAttributeValue(HIT_CHANCE) *
            (attackingPerson.getFullAttributeValue(HIT_CHANCE) + attackedPerson.getFullAttributeValue(DODGE_CHANCE)) /
            expectedDamage;
}

double Person::getLargerQuotient(double firstValue, double secondValue) {
    if (firstValue > secondValue) {
        return firstValue / secondValue;
    } else {
        return secondValue / firstValue;
    }
}

double Person::getUpProfit(Characteristic *characteristic) {
    auto expectedTurnsCount = getExpectedTurnsCount(*this, *this);
    if (!getCharacteristicCanUp(characteristic)) {
        return 0;
    }
    characteristic->upValue();
    auto expectedTurnsCountAfterUp = getExpectedTurnsCount(*this, *this);
    characteristic->debuffValue();
    return getLargerQuotient(expectedTurnsCount, expectedTurnsCountAfterUp) - 1;
}

Attribute* Person::getAttributeWithMaxUpProfitIndex() {
    auto maxProfit = 0.0;
    Attribute* attributeWithMaxProfit = nullptr;
    for (auto& attribute : attributes) {
        double attributeUpProfit = getUpProfit(attribute.get());
        if (attributeUpProfit > maxProfit) {
            maxProfit = attributeUpProfit;
            attributeWithMaxProfit = attribute.get();
        }
    }
    return attributeWithMaxProfit;
}

Skill* Person::getRandomSkill() {
    if (skills.size()) {
        return skills[rand() % skills.size()];
    }
    return nullptr;
}

void Person::save() const {
    MFile::write(attributePoints);
    for (auto &attribute : attributes) {
        attribute->save();
    }
    MFile::write(skills.size());
    for (auto &skill : skills) {
        MFile::write(skill->getId());
    }
    MFile::write(skillPoints);
    MFile::write(name.toStdString());
    MFile::write(isAlive);
}

void Person::load(Person &person) {
    person.attributePoints = MFile::readInt();
    for (auto &attribute : person.attributes) {
        attribute->load();
    }
    int SkillsCount = MFile::readInt();
    person.skills.clear();
    for (auto i = 0; i < SkillsCount; i++) {
        person.addSkill(Skill::getSkill(MFile::readInt()));
    }
    person.skillPoints = MFile::readInt();
    person.name = MFile::readString().c_str();
    person.isAlive = MFile::readInt();
}

void Person::kill() {
    isAlive = false;
}

bool Person::getIsAlive() const {
    return isAlive;
}

const std::map<int, const Skill*>& Person::getActiveSkills() const
{
    return activeSkills;
}

void Person::setCurrentActiveSkillId(int id)
{
    currentActiveSkillId = id;
}

int Person::getCurrentActiveSkillId() const
{
    return currentActiveSkillId;
}

int Person::getMana() const
{
    return mana;
}

bool Person::getActiveSkillCanUse(const Skill* skill)
{
    int currentActiveSkillId = this->currentActiveSkillId;
    this->currentActiveSkillId = skill->getId();
    for (auto i = 0; i < ATTRIBUTES_COUNT; i++) {
        if (getFullAttributeValue(AttributeIndex(i)) < AllAttributes::getAttributes()->at(i)->getMinValue()) {
            this->currentActiveSkillId = currentActiveSkillId;
            return false;
        }
    }
    this->currentActiveSkillId = currentActiveSkillId;
    return true;
}

void Person::refreshHpAndMana()
{
    auto maxHp = getFullAttributeValue(MAX_HP);
    if (hp > maxHp) {
        hp = maxHp;
    }
    auto maxMana = getFullAttributeValue(MANA);
    if (mana > maxMana) {
        mana = maxMana;
    }
}

const QString& Person::attack(Person* enemy) {
    if (currentActiveSkillId > -1) {
        mana--;
        if (!mana) {
            currentActiveSkillId = -1;
        }
    }
    int hitChange = getFullAttributeValue(HIT_CHANCE);
    int dodgeChange = enemy->getFullAttributeValue(DODGE_CHANCE);
    if (hitChange + dodgeChange == 0) {
        log.append("Ошибка: Сумма шанса попадания и шанса уворота равна 0\n");
        return log;
    }
    bool isCrit = false;
    if (rand() % (hitChange + dodgeChange) < hitChange) {
        int createChance = getFullAttributeValue(CRIT_CHANCE);
        if (rand() % 100 < createChance) {
            log.append(QString("Критический удар\n"));
            isCrit = true;
        }
        enemy->log.append(QString("Получено урона - %1\n").arg(enemy->hit(*this, isCrit)));
        if (enemy->getHp() <= 0) {
            for (auto i = 0; i < trainingSpeed; i++) {
                log.append(*Bonus::bonus(*this, true));
            }
        }
    } else {
        log.append("Промах\n");
    }
    return log;
}

void Person::upCharacteristic(Characteristic *characteristic) {
    if (characteristic == nullptr) {
        qDebug() << "characteristic == nullptr in Person::upCharacteristic";
        throw std::exception();
    }
    if (dynamic_cast<Attribute*>(characteristic)) {
        attributePoints--;
    }
    auto skill = dynamic_cast<Skill*>(characteristic);
    if (skill) {
        skillPoints--;
        if (skill->getType() == ACTIVE) {
            activeSkills[skill->getId()] = skill;
        }
    }
    characteristic->upValue();
}

const QString& Person::getLog() const {
    return log;
}

void Person::setLog(const QString &log) {
    this->log = log;
}

void Person::addSkill(Skill* skill) {
    if (skill->getValue() && skill->getType() == ACTIVE) {
        activeSkills[skill->getId()] = skill;
    }
    skills.push_back(skill);
}

int Person::getFullSkillsImpact(AttributeIndex attributeIndex, bool withCurrentActiveSkill) const {
    int result = 0;
    for (auto skill : skills) {
        if (skill->getType() == PASSIVE) {
            result += skill->getAttributeImpact(AllAttributes::getAttributes()->at(attributeIndex).get());
        }
    }
    if (withCurrentActiveSkill && currentActiveSkillId > -1) {
        result += activeSkills.at(currentActiveSkillId)->getAttributeImpact(AllAttributes::getAttributes()->at(attributeIndex).get());
    }
    return result;
}

int Person::getFullAttributeValue(AttributeIndex attributeIndex) const {
    int fullAttributeValue = attributes[attributeIndex]->getValue() + getFullSkillsImpact(attributeIndex, true);
    if (fullAttributeValue > attributes[attributeIndex]->getStaticAttribute()->getMaxValue()) {
        fullAttributeValue = attributes[attributeIndex]->getStaticAttribute()->getMaxValue();
    }
    return fullAttributeValue;
}

std::vector<Skill*>& Person::getSkills() {
    return skills;
}

void Person::addSkillPoint() {
    skillPoints++;
}

bool Person::removeSkillPoints(int removedSkillPointsCount) {
    if (skillPoints - removedSkillPointsCount >= 0) {
        skillPoints -= removedSkillPointsCount;
        return true;
    } else {
        return false;
    }
}

int &Person::getSkillPointsCount() {
    return skillPoints;
}
