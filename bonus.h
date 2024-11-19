#ifndef BONUS_H
#define BONUS_H

#include <functional>
#include <person.h>

class Bonus
{
public:
    static std::unique_ptr<QString> bonus(Person &person, bool up);
private:
    Bonus(std::function<const std::unique_ptr<QString>(Person &person, bool up)> func);
    static void initialize();

    std::function<const std::unique_ptr<QString>(Person &person, bool up)> func;
};

#endif // BONUS_H
