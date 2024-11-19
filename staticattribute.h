#ifndef STATICATTRIBUTE_H
#define STATICATTRIBUTE_H

#include <functional>
#include <QString>
#include <limits>

enum AttributeIndex {
    MAX_HP,
    DAMAGE,
    ARMOR,
    RESISTANCE,
    HIT_CHANCE,
    DODGE_CHANCE,
    CRIT_CHANCE,
    CRIT_DAMAGE,
    MANA,
    ATTRIBUTES_COUNT
};

class StaticAttribute
{
public:
    StaticAttribute(const QString &name, AttributeIndex index, int upValue, int minValue, int maxValue = std::numeric_limits<int>::max());
    const QString &getName() const;
    AttributeIndex getIndex() const;
    int getMinValue() const;
    int getMaxValue() const;
    int getUpValue() const;

private:
    QString const name;
    AttributeIndex const index;
    int upValue;
    int const minValue;
    int const maxValue;
};

#endif // STATICATTRIBUTE_H
