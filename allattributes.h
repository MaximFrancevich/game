#ifndef ALLATTRIBUTES_H
#define ALLATTRIBUTES_H

#include <memory>

#include "staticattribute.h"

class AllAttributes
{
public:
    static const std::array<std::unique_ptr<StaticAttribute>, ATTRIBUTES_COUNT>* getAttributes();
    static const StaticAttribute* getRandomStaticAttribute();
    static const StaticAttribute* getRandomAttackAttribute();

private:
    static void initialize();
};

#endif // ALLATTRIBUTES_H
