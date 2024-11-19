#ifndef SkillsWINDOW_H
#define SkillsWINDOW_H

#include <QWidget>
#include <characteristics/skills/skill.h>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "person.h"
#include "skilllayout.h"
#include "attributeswindow.h"

class SkillsWindow : public QWidget
{
    Q_OBJECT
public:
    SkillsWindow(Person &person, int currentPersonIndex);

private:
    QVBoxLayout* const mainLayout = new QVBoxLayout;
    QLabel* const skillPointsLabel = new QLabel;
    Person& person;
    int const currentPersonIndex;
    std::map<Skill*, SkillWidget*> skillWidgets;
    QWidget* skillsWidget = new QWidget;

    void refreshUpButtonsEnabled();

private slots:
    void onSkillUped();

signals:
    void uped();

public slots:
    void refresh();
};

#endif // SkillsWINDOW_H
