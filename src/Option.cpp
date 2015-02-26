#include "Option.hpp"

Option::Option() { }

Option::Option(bool value) : value(value) { }

Option::Option(const Option& other) : value(other.value) { }

Option& Option::operator=(const Option& other) {
    setValue(other.value);
    return *this;
}

bool& Option::operator=(bool value) {
    setValue(value);
    return this->value;
}

void Option::setValue(bool value) {
    bool oldValue = this->value;
    this->value = value;

    emit changed(oldValue, value);
}

bool operator==(const Option& a, const Option& b) {
    return a.value == b.value;
}

bool operator==(const Option& a, bool b) {
    return a.value == b;
}

bool operator==(bool a, const Option& b) {
    return a == b.value;
}
