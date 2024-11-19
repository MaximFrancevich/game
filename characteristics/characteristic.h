#ifndef CHARACTERISTIC_H
#define CHARACTERISTIC_H

#include "staticattribute.h"

class Characteristic
{
public:
    virtual int getValue() const;
    virtual void upValue();
    virtual bool debuffValue();
    virtual void save() const;
    virtual void load();

protected:
    Characteristic(int minValue);
    Characteristic();

    int minValue;
    int value;
};

#endif // CHARACTERISTIC_H
