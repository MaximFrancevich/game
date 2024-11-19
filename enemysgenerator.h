#ifndef ENEMYSGENERATOR_H
#define ENEMYSGENERATOR_H

#include "person.h"

class EnemiesGenerator
{
public:
    static std::unique_ptr<std::vector<Person>> generateEnemyByLevel(int level);
};

#endif // ENEMYSGENERATOR_H
