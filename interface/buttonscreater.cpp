#include "buttonscreater.h"

QPushButton* ButtonsCreater::createButton(const QString &caption, QBoxLayout *layout, const QObject *reciver, const char *slot) {
    auto button = new QPushButton(caption);
    if (layout) {
        layout->addWidget(button);
    }
    QObject::connect(button, SIGNAL(clicked()), reciver, slot);
    return button;
}

AttackButton *ButtonsCreater::createAttackButton(const QString &caption, QBoxLayout *layout, int id, const QObject *reciver)
{
    auto button = new AttackButton(caption, id);
    layout->addWidget(button);
    QObject::connect(button, SIGNAL(select(int)), reciver, SLOT(onAttackSelected(int)));
    return button;
}
