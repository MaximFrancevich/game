#ifndef QUESTLAYOUT_H
#define QUESTLAYOUT_H

#include <QLayout>
#include <QPushButton>

class QuestLayout : public QHBoxLayout
{
    Q_OBJECT
public:
    QuestLayout(int index, int difficultyLevel);

private slots:
    void onSelectButtonClicked();

private:
    int const index;

signals:
    void select(int index);
};

#endif // QUESTLAYOUT_H
