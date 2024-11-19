#include "mainwindow.h"
#include "enemysgenerator.h"
#include "allattributes.h"
#include <QCloseEvent>
#include "skillswindow.h"
#include <QSpinBox>
#include "quest.h"
#include "questlayout.h"
#include "personlayout.h"
#include <QProgressBar>
#include <QLineEdit>
#include "enemylayout.h"
#include "buttonscreater.h"
#include "fightfriendlypersonlayout.h"
#include <filesystem>
#include <QDir>
#include "record.h"
#include "attackbutton.h"
#include <QTableWidget>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <filesystem>

void MainWindow::refreshPersonsLayouts() {
    for (unsigned i = 0; i < enemyLayouts.size(); i++) {
        enemyLayouts[i]->setHpBarValue(game->getEnemys()->at(i).getHp());
        enemyLayouts[i]->setLogLabelText(game->getEnemys()->at(i).getLog());
    }
    for (auto &pair : personLayouts) {
        pair.second->setHpBarValue(pair.first->getHp());
        pair.second->setLogLabelText(pair.first->getLog());
        pair.second->setManaBarValue(pair.first->getMana());
    }
    game->clearPersonsLog();
}

void MainWindow::finishQuest() {
    game->finishQuest();
    showBase();
}

void MainWindow::nextFight() {
    game->nextFight();
    setCentralWidget(new QWidget);
    auto fightLayout = new QVBoxLayout(centralWidget());
    auto enemysLayout = new QHBoxLayout;
    fightLayout->addLayout(enemysLayout);
    enemyLayouts.clear();
    for (unsigned i = 0; i < game->getEnemys()->size(); i++) {
        auto enemyLayout = new FightEnemyLayout(game->getEnemys()->at(i), i);
        enemyLayouts.push_back(enemyLayout);
        enemysLayout->addLayout(enemyLayout);
        connect(enemyLayout, &FightEnemyLayout::attack, this, &MainWindow::attack);
    }
    nextFightButton = ButtonsCreater::createButton("Следующий бой", fightLayout, this, SLOT(nextFight()));
    nextFightButton->setEnabled(false);
    auto personsLayout = new QHBoxLayout;
    fightLayout->addLayout(personsLayout);
    personLayouts.clear();
    for (auto &person : game->getPersons()) {
        person.second.refreshHpAndMana();
        auto personLayout = new FightFriendlyPersonLayout(person.second);
        personLayouts[&person.second] = personLayout;
        fightLayout->addLayout(personLayout);
        personLayout->addWidget(new QLabel(person.second.getName()));
        attackButtons.clear();
        attackButtons[-1] = ButtonsCreater::createAttackButton("Обычная атака", personLayout, -1, this);
        currentAttackButton = attackButtons[-1];
        for (auto skill : person.second.getActiveSkills()) {
            if (!skill.second->getIsHiden() && game->getPersons().at(person.first).getActiveSkillCanUse(skill.second)) {
                attackButtons[skill.second->getId()] = ButtonsCreater::createAttackButton(skill.second->getCurrentEffectDescription()->c_str(),
                                                                                          personLayout,
                                                                                          skill.second->getId(),
                                                                                          this);
            }
        }
    }
    refreshAttackButtons();
    finishQuestButton = ButtonsCreater::createButton("Завершить задание", fightLayout, this, SLOT(finishQuest()));
}

void MainWindow::onQuestSelected(int questIndex) {
    game->startQuest(questIndex);
    nextFight();
}

void MainWindow::closePersonMenu() {
    game->setPersonName(currentPersonId, personNameLineEdit->text());
    showBase();
}

void MainWindow::showPersonMenu(int index) {
    currentPersonId = index;
    setCentralWidget(new QWidget);
    auto personLayout = new QVBoxLayout(centralWidget());
    auto nameLayout = new QHBoxLayout;
    personLayout->addLayout(nameLayout);
    nameLayout->addWidget(new QLabel("Имя:"));
    personNameLineEdit = new QLineEdit(game->getPersons().at(index).getName());
    nameLayout->addWidget(personNameLineEdit);
    ButtonsCreater::createButton("Навыки", personLayout, this, SLOT(openSkills()));
    ButtonsCreater::createButton("Назад", personLayout, this, SLOT(closePersonMenu()));
}

void MainWindow::showQuests() {
    setCentralWidget(new QWidget);
    auto questSelectLayout = new QVBoxLayout(centralWidget());
    for (auto i = 0; i < cns::questsCount; i++) {
        auto questLayout = new QuestLayout(i, game->getQuests()[i]->getDifficultyLevel());
        questSelectLayout->addLayout(questLayout);
        connect(questLayout, &QuestLayout::select, this, &MainWindow::onQuestSelected);
    }
    ButtonsCreater::createButton("Назад", questSelectLayout, this, SLOT(showBase()));
}

void MainWindow::showBase() {
    if (!game->checkAlives()) {
        showMenu();
        return;
    }
    setCentralWidget(new QWidget);
    auto baseLayout = new QVBoxLayout(centralWidget());
    auto personsLayout = new QVBoxLayout;
    baseLayout->addLayout(personsLayout);
    for (auto &person : game->getPersons()) {
        auto personLayout = new PersonLayout(person.second.getName(), person.first);
        personsLayout->addLayout(personLayout);
        connect(personLayout, &PersonLayout::personSelected, this, &MainWindow::showPersonMenu);
    }
    if (game->getPersons().size()) {
        ButtonsCreater::createButton("Начать задание", baseLayout, this, SLOT(showQuests()));
    }
    ButtonsCreater::createButton("Выход в меню", baseLayout, this, SLOT(showMenu()));
    game->save();
}

void MainWindow::onGameSettingsAccepted() {
    game = std::make_unique<Game>(difficultyLevelSpinBox->value(), nameLineEdit->text().toStdString(), trainingSpeedSpinBox->value());
    showBase();
}

void MainWindow::onNewGamePushButtonClicked() {
    setCentralWidget(new QWidget);
    auto newGameLayout = new QVBoxLayout(centralWidget());

    auto nameLayout = new QHBoxLayout;
    newGameLayout->addLayout(nameLayout);
    nameLayout->addWidget(new QLabel("Название игры"));
    nameLineEdit = new QLineEdit("Игра");
    nameLayout->addWidget(nameLineEdit);

    auto difficultyLevelLayout = new QHBoxLayout;
    newGameLayout->addLayout(difficultyLevelLayout);
    difficultyLevelLayout->addWidget(new QLabel("Уровень сложности:"));
    difficultyLevelSpinBox = new QSpinBox;
    difficultyLevelLayout->addWidget(difficultyLevelSpinBox);
    difficultyLevelSpinBox->setMinimum(1);

    auto trainingSpeedLayout = new QHBoxLayout;
    newGameLayout->addLayout(trainingSpeedLayout);
    trainingSpeedLayout->addWidget(new QLabel("Скорость обучения"));
    trainingSpeedSpinBox = new QSpinBox;
    trainingSpeedSpinBox->setMinimum(1);
    trainingSpeedSpinBox->setMaximum(9);
    trainingSpeedLayout->addWidget(trainingSpeedSpinBox);

    ButtonsCreater::createButton("Принять", newGameLayout, this, SLOT(onGameSettingsAccepted()));
    ButtonsCreater::createButton("Назад", newGameLayout, this, SLOT(showMenu()));
}

void MainWindow::loadGame() {
    setCentralWidget(new QWidget);
    auto loadGameLayout = new QVBoxLayout(centralWidget());
    loadGamesList = new QListWidget;
    loadGameLayout->addWidget(loadGamesList);
    QFileInfoList dirContent = QDir("saves").entryInfoList(QStringList() << "*.sav");
    for (auto& file : dirContent) {
        loadGamesList->addItem(file.baseName());
        connect(loadGamesList, &QListWidget::doubleClicked, this, &MainWindow::onSaveSelected);
    }
    ButtonsCreater::createButton("Загрузить", loadGameLayout, this, SLOT(onSaveSelected()));
    ButtonsCreater::createButton("Назад", loadGameLayout, this, SLOT(showMenu()));
}

void MainWindow::onSaveSelected() {
    if (loadGamesList->currentRow() > -1) {
        game = std::make_unique<Game>();
        Game::load(*game.get(), ("saves/" + loadGamesList->currentItem()->text() + ".sav").toStdString());
        showBase();
    }
}

void MainWindow::showRecords() {
    setCentralWidget(new QWidget);
    auto recordsLayout = new QVBoxLayout(centralWidget());
    auto recordsTable = new QTableWidget;
    recordsLayout->addWidget(recordsTable);
    recordsTable->setColumnCount(RECORDS_TABLE_COLUMNS_COUNT);
    recordsTable->setHorizontalHeaderLabels({ "Имя игры", "Завершено заданий", "Выполнено заданий"} );
    QFileInfoList dirContent = QDir("records").entryInfoList(QStringList() << "*.rec");
    std::vector<std::string> recordFiles;
    for (auto& file : dirContent) {
        recordFiles.push_back(file.filePath().toStdString());
    }
    auto records = Record::loadAllRecords(recordFiles);
    recordsTable->setRowCount(dirContent.count());
    for (auto i = 0; i < dirContent.count(); i++) {
        recordsTable->setItem(i, NAME, new QTableWidgetItem(records->at(i).getGameName().c_str()));
        recordsTable->setItem(i, FINISHED_QUESTS_COUNT, new QTableWidgetItem(QString::number(records->at(i).getFinishedQuestsCount())));
        recordsTable->setItem(i, COMPLIT_QUESTS_COUNT, new QTableWidgetItem(QString::number(records->at(i).getComplitQuestsCount())));
    }
    recordsTable->resizeColumnsToContents();
    recordsTable->resizeRowsToContents();
    ButtonsCreater::createButton("Назад", recordsLayout, this, SLOT(showMenu()));
}

void MainWindow::onAttackSelected(int id)
{
    if (currentAttackButton->isEnabled()) {
        currentAttackButton->setStyleSheet("color: black");
    } else {
        currentAttackButton->setStyleSheet("color: gray");
    }
    currentAttackButton = attackButtons[id];
    currentAttackButton->setStyleSheet("color: blue");
    game->getPersons().at(currentPersonId).setCurrentActiveSkillId(id);
}

void MainWindow::showCurrentPersonMenu()
{
    showPersonMenu(currentPersonId);
}

void MainWindow::showMenu() {
    setCentralWidget(new QWidget);
    auto menuLayout = new QVBoxLayout(centralWidget());
    ButtonsCreater::createButton("Новая игра", menuLayout, this, SLOT(onNewGamePushButtonClicked()));
    ButtonsCreater::createButton("Загрузить игру", menuLayout, this, SLOT(loadGame()));
    ButtonsCreater::createButton("Рекорды", menuLayout, this, SLOT(showRecords()));
    ButtonsCreater::createButton("Выход", menuLayout, this, SLOT(close()));
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{parent}
{
    showFullScreen();
    showMenu();

}

void MainWindow::openSkills() {
    setCentralWidget(new QWidget);
    auto mainLayout = new QVBoxLayout(centralWidget());
    auto personLayout = new QHBoxLayout;
    auto attributesWindow = new AttributesWindow(&game->getPersons().at(currentPersonId), currentPersonId);
    personLayout->addWidget(attributesWindow);
    auto skillsWindow = new SkillsWindow(game->getPersons().at(currentPersonId), currentPersonId);
    personLayout->addWidget(skillsWindow);
    mainLayout->addLayout(personLayout);
    connect(attributesWindow, &AttributesWindow::uped, skillsWindow, &SkillsWindow::refresh);
    connect(skillsWindow, &SkillsWindow::uped, attributesWindow, &AttributesWindow::refresh);
    ButtonsCreater::createButton("Назад", mainLayout, this, SLOT(showCurrentPersonMenu()));
    skillsWindow->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

void MainWindow::setAttackButtonsEnabled(bool enabled) {
    for (auto enemyLayout : enemyLayouts) {
        enemyLayout->setAttackButtonEnabled(enabled);
    }
}

void MainWindow::refreshAttackButtons()
{
    for (auto& button : attackButtons) {
        if (button.first == -1) {
            continue;
        }
        bool enabled = game->getPersons().at(currentPersonId).getActiveSkills().at(button.first)->getValue() *
                cns::manaForSkillLevel <=
                game->getPersons().at(currentPersonId).getMana();
        button.second->setEnabled(enabled);
        if (!enabled) {
            button.second->setStyleSheet("color: gray");
        }
    }
    onAttackSelected(game->getPersons().at(currentPersonId).getCurrentActiveSkillId());
}

void MainWindow::attack(int enemyId) {
    game->attackEnemy(currentPersonId, enemyId);
    if (game->getDeadEnemiesCount() < (int)game->getEnemys()->size()) {
        for (unsigned i = 0; i < game->getEnemys()->size(); i++) {
            if (game->getEnemys()->at(i).getHp() > 0) {
                game->getEnemy(i).attack(&game->getPersons().at(currentPersonId));
            } else {
                enemyLayouts.at(i)->setAttackButtonEnabled(false);
            }
        }
        if (game->getPersons().at(currentPersonId).getHp() <= 0) {
            setAttackButtonsEnabled(false);
        }
    } else {
        setAttackButtonsEnabled(false);
        if (game->getCurrentFightNumber() == cns::fightsInQuest) {
            game->complitQuest();
        } else {
            nextFightButton->setEnabled(true);
        }
    }
    refreshPersonsLayouts();
    refreshAttackButtons();
}
