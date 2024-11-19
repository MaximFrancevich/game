#include "skill.h"
#include "allattributes.h"
#include "constants.h"
#include "fstream"
#include "mfile.h"
#include <map>

static std::vector<std::unique_ptr<Skill>>* skills = nullptr;

Skill::Skill(SkillType skillType, int id, int level) :
    Characteristic(0),
    skillType { skillType },
    id { id },
    level { level }
{
    attributeImpacts.fill(0);
    auto getRandomAttribute =
            skillType == ACTIVE
            ? &AllAttributes::getRandomAttackAttribute :
              &AllAttributes::getRandomStaticAttribute;
    int currentLevel = 0;
    while (currentLevel <= level) {
        int impact = rand() % cns::maxSkillAttributeImpact + 1;
        attributeImpacts[getRandomAttribute()->getIndex()] += impact;
        currentLevel += impact;
    }
    attributeImpacts[getRandomAttribute()->getIndex()] += level - currentLevel;
}

Skill::Skill(int id) : id { id }
{

}

Skill::Skill() : Characteristic() {

}

const std::unique_ptr<std::string> Skill::getCurrentEffectDescription() const
{
    return getEffectDescription(false);
}

const std::unique_ptr<std::string> Skill::getNextLevelEffectDescription() const
{
    return getEffectDescription(true);
}

void Skill::save() const {
    MFile::write(skillType);
    Characteristic::save();
    if (parent) {
        MFile::write(parent->getId());
    } else {
        MFile::write(-1);
    }
    for (auto& impact : attributeImpacts) {
        MFile::write(impact);
    }
    MFile::write(isHiden);
}

void Skill::load() {
    Characteristic::load();
    int parentId = MFile::readInt();
    if (parentId > -1) {
        parent = skills->at(parentId).get();
        level = skills->at(parentId)->level + 1;
    }
    for (auto& impact : attributeImpacts) {
        impact = MFile::readInt();
    }
    isHiden = MFile::readInt();
}

SkillType Skill::getType() const {
    return skillType;
}

void Skill::setParent(Skill *parent)
{
    this->parent = parent;
    if (parent) {
        parent->addChild(this);
    }
}

void Skill::removeLastChild()
{
    children.pop_back();
}

Skill* Skill::getParent() const
{
    return parent;
}

int Skill::getId() const
{
    return id;
}

void Skill::addChild(const Skill* child)
{
    this->children.push_back(child);
}

const std::unique_ptr<std::string> Skill::getEffectDescription(bool nextLevel) const
{
    auto result = std::make_unique<std::string>();
    for (unsigned i = 0; i < attributeImpacts.size(); i++) {
        if (attributeImpacts[i]) {
            result->append(AllAttributes::getAttributes()->at(i)->getName().toStdString());
            int attributeImpact = getAttributeImpact(AllAttributes::getAttributes()->at(i).get());
            if (nextLevel) {
                attributeImpact += AllAttributes::getAttributes()->at(i)->getUpValue() * attributeImpacts[i];
            }
            if (attributeImpacts[i] > 0) {
                result->append(" + " + std::to_string(attributeImpact) + "\n");
            } else {
                result->append(" - " + std::to_string(-attributeImpact) + "\n");
            }
        }
    }
    return result;
}

Skill* Skill::generateSkill(int level) {
    auto skillType = rand() % ((1 + SKILLS_TYPES_COUNT) * SKILLS_TYPES_COUNT / 2);
    skillType = (sqrt(1 + 8 * skillType) - 1) / 2;
    return generateSkillByType((SkillType)skillType, level);
}

Skill* Skill::generateSkillByType(SkillType skillType, int level) {
    if (!skills) {
        skills = new std::vector<std::unique_ptr<Skill>>;
    }
    skills->push_back(std::make_unique<Skill>(skillType, skills->size(), level));
    return skills->at(skills->size() - 1).get();
}

void Skill::saveAllSkills()
{
    if (skills) {
        MFile::write(skills->size());
        for (auto& skill : *skills) {
            skill->save();
        }
    } else {
        MFile::write(0);
    }
}

void Skill::loadAllSkills() {
    if (!skills) {
        skills = new std::vector<std::unique_ptr<Skill>>;
    }
    skills->clear();
    int skillsCount = MFile::readInt();
    for (auto i = 0; i < skillsCount; i++) {
        generateSkillByType((SkillType)MFile::readInt(), 1);
        skills->at(skills->size() - 1)->load();
    }
}

Skill *Skill::getSkill(int id)
{
    return skills->at(id).get();
}

int Skill::getLevel() const
{
    return level;
}

const std::array<int, ATTRIBUTES_COUNT>& Skill::getAttributeImpacts() const
{
    return attributeImpacts;
}

void Skill::setIsHiden(int isHiden)
{
    this->isHiden = isHiden;
}

bool Skill::getIsHiden() const
{
    return isHiden;
}

int Skill::getAttributeImpact(const StaticAttribute* attribute) const
{
    return value * attribute->getUpValue() * attributeImpacts[attribute->getIndex()];
}
