#ifndef PERSONLAYOUT_H
#define PERSONLAYOUT_H

#include <QHBoxLayout>

class PersonLayout : public QHBoxLayout
{
    Q_OBJECT
public:
    PersonLayout(const QString &personName, int index);

private slots:
    void onPersonButtonClicked();

private:
    int index;

signals:
    void personSelected(int index);
};

#endif // PERSONLAYOUT_H
