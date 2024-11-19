#include "enemysgenerator.h"
#include <iostream>
#include "allattributes.h"
#include <QDebug>

std::unique_ptr<std::vector<Person>> EnemiesGenerator::generateEnemyByLevel(int level) {
    int maxAttributeLevel = level * 2;
    auto enemies = std::make_unique<std::vector<Person>>();
    for (auto i = 0; i < rand() % cns::enemiesInFightMax + 1; i++) {
        enemies->push_back(Person(0));
        if (maxAttributeLevel) {
            for (auto &attribute : enemies->at(i).getAttributes()) {
                int attributeLevel = rand() % maxAttributeLevel;
                attribute->setValue(attribute->getStaticAttribute()->getMinValue() +
                                    attributeLevel * attribute->getStaticAttribute()->getUpValue());
            }
        }
        enemies->at(i).setHpAndManaToMax();
    }
    return enemies;
}
