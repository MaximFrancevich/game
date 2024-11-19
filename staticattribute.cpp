#include "staticattribute.h"

StaticAttribute::StaticAttribute(const QString &name, AttributeIndex index, int upValue, int minValue, int maxValue) :
    name { name }
  , index { index }
  , upValue { upValue }
  , minValue { minValue }
  , maxValue { maxValue }
{

}

AttributeIndex StaticAttribute::getIndex() const {
    return index;
}

const QString &StaticAttribute::getName() const {
    return name;
}

int StaticAttribute::getMinValue() const {
    return minValue;
}

int StaticAttribute::getMaxValue() const {
    return maxValue;
}

int StaticAttribute::getUpValue() const
{
    return upValue;
}
