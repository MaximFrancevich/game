#ifndef SKILLLAYOUT_H
#define SKILLLAYOUT_H

#include <QVBoxLayout>
#include <QWidget>
#include "characteristics/skills/skill.h"
#include <QGroupBox>
#include "person.h"
#include <QLabel>
#include <QPushButton>

class SkillWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit SkillWidget(Skill* skill, Person& person);
    void addChild(SkillWidget* widget);
    void refreshUpButtonEnabled();

private:
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QWidget* childrenWidget = new QWidget;
    Person& person;
    Skill* skill;
    QLabel* levelLabel = new QLabel;
    QLabel* effectDescriptionLabel = new QLabel;
    QPushButton* upButton = new QPushButton("+");
    QLabel* nextLevelEffectDescriptionLabel = new QLabel;

    void addWidget(QWidget* widget);
    void refresh();

private slots:
    void onUpButtonClicked();
    void onHideCheckBoxStateChanged(int state);

signals:
    void uped();
};

#endif // SKILLLAYOUT_H
