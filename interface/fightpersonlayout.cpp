#include "fightpersonlayout.h"

FightPersonLayout::FightPersonLayout(const Person &person) : QVBoxLayout()
{
    hpBar->setMaximum(person.getFullAttributeValue(MAX_HP));
    hpBar->setValue(person.getHp());
    hpBar->setStyleSheet("QProgressBar::chunk { background: red }");
    hpBar->setAlignment(Qt::AlignCenter);
    manaBar->setMaximum(person.getFullAttributeValue(MANA));
    manaBar->setValue(person.getMana());
    addWidget(hpBar);
    addWidget(manaBar);
    addWidget(logLabel);
}

void FightPersonLayout::setHpBarValue(int value) {
    hpBar->setValue(value);
}

void FightPersonLayout::setLogLabelText(const QString &text) {
    logLabel->setText(text);
}

void FightPersonLayout::setManaBarValue(int value)
{
    manaBar->setValue(value);
}
