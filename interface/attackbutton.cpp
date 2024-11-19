#include "attackbutton.h"

AttackButton::AttackButton(const QString& caption, int id, QWidget* parent) :
    QPushButton(caption, parent),
    id { id }
{
    connect(this, &QPushButton::clicked, this, &AttackButton::onClicked);
}

void AttackButton::onClicked()
{
    emit select(id);
}
