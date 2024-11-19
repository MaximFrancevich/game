#ifndef SKILL_H
#define SKILL_H

#include <memory>
#include "characteristics/baseattribute.h"
#include <map>

enum SkillType {
    ACTIVE,
    PASSIVE,
    SKILLS_TYPES_COUNT
};

class Skill : public Characteristic
{
public:
    Skill(SkillType skillType, int id, int level);
    Skill(int id);
    Skill();
    const std::unique_ptr<std::string> getCurrentEffectDescription() const;
    const std::unique_ptr<std::string> getNextLevelEffectDescription() const;
    virtual void load() override;
    virtual void save() const override;
    SkillType getType() const;
    void setParent(Skill* parent);
    void removeLastChild();
    Skill* getParent() const;
    int getId() const;
    static Skill* generateSkill(int level);
    static Skill* generateSkillByType(SkillType skillType, int level);
    static void saveAllSkills();
    static void loadAllSkills();
    static Skill* getSkill(int id);
    int getLevel() const;
    const std::array<int, ATTRIBUTES_COUNT>& getAttributeImpacts() const;
    void setIsHiden(int isHiden);
    bool getIsHiden() const;
    int getAttributeImpact(const StaticAttribute* attribute) const;

private:
    void addChild(const Skill* child);
    const std::unique_ptr<std::string> getEffectDescription(bool nextLevel) const;

    SkillType skillType;
    Skill* parent = nullptr;
    std::vector<const Skill*> children;
    int id;
    std::array<int, ATTRIBUTES_COUNT> attributeImpacts;
    int level;
    bool isHiden = false;
};

#endif // SKILL_H
