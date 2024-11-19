#ifndef ATTRIBUTESWINDOW_H
#define ATTRIBUTESWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

#include "person.h"
#include "playerattribute.h"

class AttributesWindow : public QWidget
{
    Q_OBJECT
public:
    explicit AttributesWindow(Person* player, int currentPersonIndex, QWidget* parent = nullptr);

private:
    QLabel* const skillPointsLabel;
    QVBoxLayout* const playersAttributesLayout;
    Person* const player;
    int currentPersonIndex;

    std::array<std::unique_ptr<PlayerAttributePanel>, ATTRIBUTES_COUNT> playerAttributePanels;

public slots:
    void refresh();

private slots:
    void on_uped();
    void showEvent(QShowEvent* event) override;

signals:
    void uped();
};

#endif // ATTRIBUTESWINDOW_H
