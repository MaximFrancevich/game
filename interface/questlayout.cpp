#include "questlayout.h"
#include <QLabel>

QuestLayout::QuestLayout(int index, int difficultyLevel) : QHBoxLayout(), index { index }
{
    auto selectButton = new QPushButton("Выбрать");
    addWidget(selectButton);
    addWidget(new QLabel("Сложность - " + QString::number(difficultyLevel)));
    connect(selectButton, &QPushButton::clicked, this, &QuestLayout::onSelectButtonClicked);
}

void QuestLayout::onSelectButtonClicked() {
    emit select(index);
}
