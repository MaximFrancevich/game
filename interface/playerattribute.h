#ifndef PLAYERATTRIBUTE_H
#define PLAYERATTRIBUTE_H

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "person.h"

class PlayerAttributePanel : public QObject
{
    Q_OBJECT
public:
    PlayerAttributePanel(Attribute &attribute, QWidget* parent, Person &person);
    void up();
    QHBoxLayout* getLayout() const;
    QPushButton* getUpButton() const;
    QLabel* getLabel() const;
    void showInfo() const;
    const QString &getName() const;
    const Attribute &getAttribute() const;

private:
    QHBoxLayout* const layout;
    QPushButton* const upButton;
    QLabel* const label;
    Attribute& attribute;
    Person& person;

signals:
    void uped();
};

#endif // PLAYERATTRIBUTE_H
