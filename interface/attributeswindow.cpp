#include "attributeswindow.h"
#include <QCloseEvent>

AttributesWindow::AttributesWindow(Person* player, int currentPersonIndex, QWidget* parent)
    : QWidget { parent }
    , skillPointsLabel { new QLabel(this, Qt::Widget) }
    , playersAttributesLayout { new QVBoxLayout }
    , player { player }
    , currentPersonIndex { currentPersonIndex }
{
    setLayout(playersAttributesLayout);
    for (auto i = 0; i < ATTRIBUTES_COUNT; i++) {
        playerAttributePanels[i] = std::make_unique<PlayerAttributePanel>(*player->getAttributes().at(i).get(), this, *player);
    }
    playersAttributesLayout->addWidget(skillPointsLabel);
    for (auto &attribute : playerAttributePanels) {
        playersAttributesLayout->addLayout(attribute->getLayout());
        connect(attribute.get(), &PlayerAttributePanel::uped, this, &AttributesWindow::on_uped);
    }
    refresh();
    showFullScreen();
}

void AttributesWindow::on_uped() {
    refresh();
    emit uped();
}

void AttributesWindow::refresh() {
    skillPointsLabel->setText(QString("Очки обучения - %1").arg(player->getAttributePointsCount()));
    for (auto &playerAttribute : playerAttributePanels) {
        playerAttribute->getUpButton()->setEnabled(player->getAttributePointsCount());
        playerAttribute->showInfo();
        playerAttribute->getLabel()->setStyleSheet("color: rgb(0, 0, 0)");
    }
    playerAttributePanels[player->getAttributeWithMaxUpProfitIndex()->getStaticAttribute()->getIndex()]
            ->getLabel()
            ->setStyleSheet("color: rgb(0, 0, 255)");
}

void AttributesWindow::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);
    refresh();
}
