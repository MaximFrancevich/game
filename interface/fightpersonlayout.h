#ifndef FIGHTPERSONLAYOUT_H
#define FIGHTPERSONLAYOUT_H

#include <QProgressBar>
#include "person.h"
#include <QVBoxLayout>
#include <QLabel>

class FightPersonLayout : public QVBoxLayout
{
public:
    FightPersonLayout(const Person &person);
    void setHpBarValue(int value);
    void setLogLabelText(const QString& text);
    void setManaBarValue(int value);

protected:
    QProgressBar* hpBar = new QProgressBar;
    QLabel* logLabel = new QLabel;
    QProgressBar* manaBar = new QProgressBar;
};

#endif // FIGHTPERSONLAYOUT_H
