#ifndef QUEST_H
#define QUEST_H


class Quest
{
public:
    Quest(int difficultyLevel);
    int getDifficultyLevel() const;

private:
    int const difficultyLevel;
};

#endif // QUEST_H
