/*****************************************
 * Harrison Healey - hmhealey - 20376857 *
 *****************************************/

#include "Option.hpp"

BooleanOption::BooleanOption() { }

BooleanOption::BooleanOption(bool value) : value(value) { }

BooleanOption::BooleanOption(const BooleanOption& other) : value(other.value) { }

BooleanOption& BooleanOption::operator=(const BooleanOption& other) {
    setValue(other.value);
    return *this;
}

bool& BooleanOption::operator=(bool value) {
    setValue(value);
    return this->value;
}

BooleanOption::operator bool() const {
    return value;
}

bool BooleanOption::toggle() {
    setValue(!value);
    return value;
}

void BooleanOption::setValue(bool value) {
    bool oldValue = this->value;
    this->value = value;

    emit changed(oldValue, value);
}

bool operator==(const BooleanOption& a, const BooleanOption& b) {
    return a.value == b.value;
}

bool operator==(const BooleanOption& a, bool b) {
    return a.value == b;
}

bool operator==(bool a, const BooleanOption& b) {
    return a == b.value;
}
