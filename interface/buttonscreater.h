#ifndef BUTTONSCREATER_H
#define BUTTONSCREATER_H

#include "attackbutton.h"
#include <QBoxLayout>

class ButtonsCreater
{
public:
    static QPushButton* createButton(const QString& caption, QBoxLayout* layout, const QObject *reciver, const char* slot);
    static AttackButton* createAttackButton(const QString& caption, QBoxLayout* layout, int id, const QObject *reciver);
};

#endif // BUTTONSCREATER_H
