#include "baseattribute.h"
#include "fstream"
#include "allattributes.h"
#include "mfile.h"

Attribute::Attribute(const StaticAttribute* staticAttribute) :
    Characteristic(staticAttribute->getMinValue()),
    staticAttribute { staticAttribute }
{

}

void Attribute::setValue(int value) {
    this->value = value;
}

const StaticAttribute* Attribute::getStaticAttribute() const
{
    return staticAttribute;
}

void Attribute::save() const
{
    Characteristic::save();
    MFile::write(staticAttribute->getIndex());
}

void Attribute::load()
{
    Characteristic::load();
    staticAttribute = AllAttributes::getAttributes()->at(MFile::readInt()).get();
}

void Attribute::upValue()
{
    value += staticAttribute->getUpValue();
}

bool Attribute::debuffValue()
{
    if (value - staticAttribute->getUpValue() < minValue) {
        return false;
    } else {
        value -= staticAttribute->getUpValue();
        return true;
    }
}
