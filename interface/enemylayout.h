#ifndef ENEMYLAYOUT_H
#define ENEMYLAYOUT_H

#include <QVBoxLayout>
#include <QProgressBar>
#include "person.h"
#include <QLabel>
#include "fightpersonlayout.h"
#include <QPushButton>

class FightEnemyLayout : public FightPersonLayout
{
    Q_OBJECT
public:
    FightEnemyLayout(const Person& enemy, int index);
    void setAttackButtonEnabled(bool enabled);

private slots:
    void onAttackButtonClicked();

signals:
    void attack(int index);

private:
    int const index;
    QPushButton *attackButton;
};

#endif // ENEMYLAYOUT_H
