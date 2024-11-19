#ifndef BASEATTRIBUTE_H
#define BASEATTRIBUTE_H

#include "characteristic.h"

class Attribute : public Characteristic
{
public:
    Attribute(const StaticAttribute* staticAttribute);
    void setValue(int value);
    const StaticAttribute* getStaticAttribute() const;
    virtual void save() const override;
    virtual void load() override;
    virtual void upValue() override;
    virtual bool debuffValue() override;

private:
    const StaticAttribute* staticAttribute;
};

#endif // BASEATTRIBUTE_H
