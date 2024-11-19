#include "playerattribute.h"

PlayerAttributePanel::PlayerAttributePanel(Attribute &attribute, QWidget* parent, Person &person) :
    layout { new QHBoxLayout }
  , upButton { new QPushButton("+", parent) }
  , label { new QLabel(parent, Qt::Widget) }
  , attribute { attribute }
  , person { person }
{
    layout->addWidget(label);
    layout->addWidget(upButton);
    parent->layout()->addItem(layout);
    connect(upButton, &QPushButton::clicked, this, &PlayerAttributePanel::up);
}

void PlayerAttributePanel::up() {
    person.upCharacteristic(&attribute);
    emit uped();
}

QHBoxLayout* PlayerAttributePanel::getLayout() const {
    return layout;
}

QPushButton* PlayerAttributePanel::getUpButton() const {
    return upButton;
}

QLabel* PlayerAttributePanel::getLabel() const {
    return label;
}

void PlayerAttributePanel::showInfo() const {
    QString labelText = QString("%1 - %2")
            .arg(attribute.getStaticAttribute()->getName())
            .arg(attribute.getValue());
    auto fullSkillsBonus = person.getFullSkillsImpact(attribute.getStaticAttribute()->getIndex(), false);
    if (fullSkillsBonus > 0) {
        labelText += QString("(+%1)").arg(fullSkillsBonus);
    }
    if (fullSkillsBonus < 0) {
        labelText += QString("(%1)").arg(fullSkillsBonus);
    }
    label->setText(labelText);
}

const QString &PlayerAttributePanel::getName() const {
    return attribute.getStaticAttribute()->getName();
}

const Attribute &PlayerAttributePanel::getAttribute() const {
    return attribute;
}
