#ifndef ATTACKBUTTON_H
#define ATTACKBUTTON_H

#include <QPushButton>

class AttackButton : public QPushButton
{
    Q_OBJECT
public:
    explicit AttackButton(const QString& caption, int id, QWidget* parent = nullptr);

private:
    const int id;

private slots:
    void onClicked();

signals:
    void select(int id);
};

#endif // ATTACKBUTTON_H
