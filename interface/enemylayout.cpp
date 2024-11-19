#include "enemylayout.h"
#include "buttonscreater.h"

FightEnemyLayout::FightEnemyLayout(const Person& enemy, int index) :
    FightPersonLayout(enemy),
    index { index }
{
    attackButton = ButtonsCreater::createButton("Атаковать", this, this, SLOT(onAttackButtonClicked()));
}

void FightEnemyLayout::onAttackButtonClicked() {
    emit attack(index);
}

void FightEnemyLayout::setAttackButtonEnabled(bool enabled) {
    attackButton->setEnabled(enabled);
}
