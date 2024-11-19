#include "allattributes.h"

static std::array<std::unique_ptr<StaticAttribute>, ATTRIBUTES_COUNT>* attributes;
static bool isInitialized = false;
static std::vector<StaticAttribute*>* attackAttributes;

const std::array<std::unique_ptr<StaticAttribute>, ATTRIBUTES_COUNT>* AllAttributes::getAttributes() {
    if (!isInitialized) {
        initialize();
    }
    return attributes;
}

void AllAttributes::initialize() {
    attributes = new std::array<std::unique_ptr<StaticAttribute>, ATTRIBUTES_COUNT>;
    attributes->at(MAX_HP) = std::make_unique<StaticAttribute>("Максимальное здоровье", MAX_HP, 15, 15);
    attributes->at(DAMAGE) = std::make_unique<StaticAttribute>("Урон", DAMAGE, 7, 7);
    attributes->at(ARMOR) = std::make_unique<StaticAttribute>("Защита", ARMOR, 3, 0);
    attributes->at(RESISTANCE) = std::make_unique<StaticAttribute>("Сопротивление", RESISTANCE, 1, 0, 90);
    attributes->at(HIT_CHANCE) = std::make_unique<StaticAttribute>("Шанс попасть", HIT_CHANCE, 1, 1);
    attributes->at(DODGE_CHANCE) = std::make_unique<StaticAttribute>("Шанс увернуться", DODGE_CHANCE, 1, 0);
    attributes->at(CRIT_CHANCE) = std::make_unique<StaticAttribute>("Шанс критического удара", CRIT_CHANCE, 1, 0, 100);
    attributes->at(CRIT_DAMAGE) = std::make_unique<StaticAttribute>("Критический урон", CRIT_DAMAGE, 10, 0);
    attributes->at(MANA) = std::make_unique<StaticAttribute>("Мана", MANA, 1, 0);

    attackAttributes = new std::vector<StaticAttribute*>;
    attackAttributes->push_back(attributes->at(DAMAGE).get());
    attackAttributes->push_back(attributes->at(HIT_CHANCE).get());
    attackAttributes->push_back(attributes->at(CRIT_CHANCE).get());
    attackAttributes->push_back(attributes->at(CRIT_DAMAGE).get());

    isInitialized = true;
}

const StaticAttribute* AllAttributes::getRandomStaticAttribute() {
    if (!isInitialized) {
        initialize();
    }
    return attributes->at(rand() % ATTRIBUTES_COUNT).get();
}

const StaticAttribute* AllAttributes::getRandomAttackAttribute() {
    if (!isInitialized) {
        initialize();
    }
    return attackAttributes->at(rand() % attackAttributes->size());
}
