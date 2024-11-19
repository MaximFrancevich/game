#include "skilllayout.h"
#include <QLabel>
#include <QPushButton>
#include "buttonscreater.h"
#include "constants.h"
#include <QCheckBox>

static int widgetNumber = 0;

SkillWidget::SkillWidget(Skill* skill, Person& person) :
    QGroupBox(),
    person { person },
    skill { skill }
{
    addWidget(new QLabel(skill->getType() == ACTIVE ? "Активный" : "Пассивный"));
    addWidget(levelLabel);
    addWidget(new QLabel("Текущий эффект:"));
    addWidget(effectDescriptionLabel);
    addWidget(new QLabel("Следующий уровень:"));
    addWidget(nextLevelEffectDescriptionLabel);
    addWidget(upButton);
    if (skill->getType() == ACTIVE) {
        auto hideCheckBox = new QCheckBox("Скрыть");
        addWidget(hideCheckBox);
        connect(hideCheckBox, &QCheckBox::stateChanged, this, &SkillWidget::onHideCheckBoxStateChanged);
        hideCheckBox->setChecked(skill->getIsHiden());
    }
    connect(upButton, &QPushButton::clicked, this, &SkillWidget::onUpButtonClicked);
    mainLayout->addWidget(childrenWidget);
    childrenWidget->setLayout(new QHBoxLayout);
    setLayout(mainLayout);
    refresh();
}

void SkillWidget::addChild(SkillWidget *widget)
{
    childrenWidget->layout()->addWidget(widget);
}

void SkillWidget::refreshUpButtonEnabled()
{
    upButton->setEnabled(person.getSkillPointsCount() && person.getCharacteristicCanUp(skill));
}

void SkillWidget::addWidget(QWidget* widget)
{
    widget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    widget->setGeometry(0, widgetNumber * cns::skillWidgetHeight, cns::skillWidgetWidth, cns::skillWidgetHeight);
    mainLayout->addWidget(widget);
    widgetNumber++;
}

void SkillWidget::refresh()
{
    levelLabel->setText(QString("Уровень - %1").arg(skill->getValue()));
    effectDescriptionLabel->setText(skill->getCurrentEffectDescription()->c_str());
    nextLevelEffectDescriptionLabel->setText(skill->getNextLevelEffectDescription()->c_str());
}

void SkillWidget::onUpButtonClicked()
{
    person.upCharacteristic(skill);
    refresh();
    emit uped();
}

void SkillWidget::onHideCheckBoxStateChanged(int state)
{
    skill->setIsHiden(state);
}
