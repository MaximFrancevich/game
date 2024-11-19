#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>

#include "person.h"
#include "attributeswindow.h"
#include <QSpinBox>
#include "constants.h"
#include "quest.h"
#include "enemylayout.h"
#include "fightfriendlypersonlayout.h"
#include "game.h"
#include <QListWidget>
#include "attackbutton.h"

enum RecordsTableColumns {
    NAME,
    FINISHED_QUESTS_COUNT,
    COMPLIT_QUESTS_COUNT,
    RECORDS_TABLE_COLUMNS_COUNT
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

signals:

private:
    std::unique_ptr<Game> game;
    int currentPersonId = 0;
    QPushButton* nextFightButton;
    QSpinBox* difficultyLevelSpinBox;
    QLineEdit *personNameLineEdit;
    std::vector<FightEnemyLayout*> enemyLayouts;
    std::map<const Person*, FightFriendlyPersonLayout*> personLayouts;
    QPushButton* finishQuestButton;
    QLineEdit* nameLineEdit;
    QListWidget* loadGamesList;
    AttackButton* currentAttackButton;
    std::map<int, AttackButton*> attackButtons;
    QSpinBox* trainingSpeedSpinBox;

    void refreshPersonsLayouts();
    void setAttackButtonsEnabled(bool enabled);
    void refreshAttackButtons();

private slots:
    void attack(int enemyIndex);
    void nextFight();
    void openSkills();
    void onNewGamePushButtonClicked();
    void showMenu();
    void onGameSettingsAccepted();
    void onQuestSelected(int questIndex);
    void showPersonMenu(int index);
    void showBase();
    void showQuests();
    void closePersonMenu();
    void finishQuest();
    void loadGame();
    void onSaveSelected();
    void showRecords();
    void onAttackSelected(int id);
    void showCurrentPersonMenu();
};

#endif // MAINWINDOW_H
