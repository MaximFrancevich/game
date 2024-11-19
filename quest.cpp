#include "quest.h"

Quest::Quest(int difficultyLevel) :
    difficultyLevel { difficultyLevel }
{

}

int Quest::getDifficultyLevel() const {
    return difficultyLevel;
}
