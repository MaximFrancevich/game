#include "bonus.h"
#include "allattributes.h"
#include <QDebug>

static QVector<Bonus>* bonuses = nullptr;
static bool isInitialized = false;

Bonus::Bonus(std::function<const std::unique_ptr<QString>(Person &person, bool up)> func) :
    func { func }
{

}

void Bonus::initialize() {
    bonuses = new QVector<Bonus>;
    bonuses->push_back(Bonus([] (Person &person, bool up) {
                           if (up) {
                               return std::make_unique<QString>("");
                           } else {
                               person.kill();
                               return std::make_unique<QString>(QString("%1 погиб\n").arg(person.getName()));
                           }
                       }));
    bonuses->push_back(Bonus([] (Person &person, bool up) {
                           if (up) {
                               int parentId = rand() % (person.getSkills().size() + 1) - 1;
                               int level = parentId == -1 ? 1 : person.getSkills().at(parentId)->getLevel() + 1;
                               auto skill = Skill::generateSkill(level);
                               skill->setParent(parentId == -1 ? nullptr : person.getSkills().at(parentId));
                               person.addSkill(skill);
                               return std::make_unique<QString>("Открыт новый навык\n");
                           } else {
                               return std::make_unique<QString>("");
                           }
                       }));
    bonuses->push_back(Bonus([] (Person &person, bool up) {
                           if (up) {
                               person.addSkillPoint();
                               return std::make_unique<QString>("Получено очко навыка\n");
                           } else {
                               if (person.removeSkillPoints()) {
                                   return std::make_unique<QString>("Потеряно очко навыка\n");
                               } else {
                                   return std::make_unique<QString>("");
                               }
                           }
                       }));
    bonuses->push_back(Bonus([] (Person &person, bool up) {
                           auto skill = person.getRandomSkill();
                           if (skill) {
                               if (up) {
                                   if (person.getCharacteristicCanUp(skill)) {
                                       skill->upValue();
                                       return std::make_unique<QString>("Изучен случайный навык\n");
                                   }
                               } else {
                                   if (skill->debuffValue()) {
                                       return std::make_unique<QString>("Потерян уровень случайного навыка\n");
                                   }
                               }
                           }
                           return std::make_unique<QString>("");
                       }));
    bonuses->push_back(Bonus([] (Person &person, bool up) {
                           if (up) {
                               person.addAttributePoint();
                               return std::make_unique<QString>("Получено очко атрибута\n");
                           } else {
                               if (person.removeAttributePoint()) {
                                   return std::make_unique<QString>("Потеряно очко атрибута\n");
                               } else {
                                   return std::make_unique<QString>("");
                               }
                           }
                       }));
    bonuses->push_back(Bonus([] (Person &person, bool up) {
                           auto attribute = AllAttributes::getRandomStaticAttribute();
                           if (up) {
                               person.getAttributes()[attribute->getIndex()]->upValue();
                               return std::make_unique<QString>(QString("Атрибут \"%1\" увеличен\n").arg(attribute->getName()));
                           } else {
                               if (person.getAttributes()[attribute->getIndex()]->debuffValue()) {
                                   return std::make_unique<QString>(QString("Атрибут \"%1\" уменьшен\n").arg(attribute->getName()));
                               } else {
                                   return std::make_unique<QString>("");
                               }
                           }
                       }));
    bonuses->push_back(Bonus([] (Person &, bool) {
                           return std::make_unique<QString>("");
                       }));
    isInitialized = true;
}

std::unique_ptr<QString> Bonus::bonus(Person &person, bool up) {
    if (!isInitialized) {
        initialize();
    }
    int currentBonusNumber = rand() % ((bonuses->length() + 1) * bonuses->length() / 2);
    currentBonusNumber = (sqrt(1 + 8 * currentBonusNumber) - 1) / 2;
    if (up) {
        return bonuses->at(currentBonusNumber).func(person, up);
    } else {
        return bonuses->at(bonuses->length() - 1 - currentBonusNumber).func(person, up);
    }
}
