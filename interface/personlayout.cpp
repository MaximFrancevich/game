#include "personlayout.h"
#include <QPushButton>

PersonLayout::PersonLayout(const QString &personName, int index) : QHBoxLayout(), index { index }
{
    auto personButton = new QPushButton(personName);
    addWidget(personButton);
    connect(personButton, &QPushButton::clicked, this, &PersonLayout::onPersonButtonClicked);
}

void PersonLayout::onPersonButtonClicked() {
    emit personSelected(index);
}
