#include "skillswindow.h"
#include "QHBoxLayout"
#include <QLabel>
#include "allattributes.h"
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include "skilllayout.h"
#include "buttonscreater.h"
#include <QScrollArea>

SkillsWindow::SkillsWindow(Person &person, int currentPersonIndex) :
    person { person },
    currentPersonIndex { currentPersonIndex }
{
    auto skillsWidget = new QWidget;
    auto skillsLayout = new QHBoxLayout;
    for (auto& skill : person.getSkills()) {
        skillWidgets[skill] = new SkillWidget(skill, person);
        connect(skillWidgets[skill], &SkillWidget::uped, this, &SkillsWindow::onSkillUped);
        if (skill->getParent()) {
            skillWidgets[skill->getParent()]->addChild(skillWidgets[skill]);
        } else {
            skillsLayout->addWidget(skillWidgets[skill]);
        }
    }
    skillsWidget->setLayout(skillsLayout);
    auto scrollArea = new QScrollArea;
    scrollArea->setWidget(skillsWidget);
    mainLayout->addWidget(scrollArea);
    mainLayout->addWidget(skillPointsLabel);
    setLayout(mainLayout);
    refresh();
}

void SkillsWindow::refresh() {
    refreshUpButtonsEnabled();
    skillPointsLabel->setText(QString("Очки навыков - %1").arg(person.getSkillPointsCount()));
}

void SkillsWindow::onSkillUped() {
    refresh();
    emit uped();
}

void SkillsWindow::refreshUpButtonsEnabled() {
    for (auto &skillWidget : skillWidgets) {
        skillWidget.second->refreshUpButtonEnabled();
    }
}
