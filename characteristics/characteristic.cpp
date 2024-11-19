#include "characteristic.h"
#include "fstream"
#include "allattributes.h"
#include <QDebug>
#include "mfile.h"

Characteristic::Characteristic(int minValue) :
    minValue { minValue },
    value { minValue }
{

}

Characteristic::Characteristic() {

}

int Characteristic::getValue() const {
    return value;
}

void Characteristic::upValue() {
    value++;
}

bool Characteristic::debuffValue() {
    if (value - 1 < minValue) {
        return false;
    }
    value--;
    return true;
}

void Characteristic::save() const {
    MFile::write(minValue);
    MFile::write(value);
}

void Characteristic::load() {
    minValue = MFile::readInt();
    value = MFile::readInt();
}
