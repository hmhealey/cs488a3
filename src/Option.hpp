#ifndef __OPTION_HPP__
#define __OPTION_HPP__

#include <QObject>

class Option : QObject {
    Q_OBJECT

public:
    bool value;

    Option();
    Option(bool value);
    Option(const Option& other);

    Option& operator=(const Option& other);
    bool& operator=(bool value);

    void setValue(bool value);

signals:
    void changed(bool oldValue, bool newValue);

};

bool operator==(const Option& a, const Option& b);

bool operator==(const Option& a, bool b);

bool operator==(bool a, const Option& b);

#endif
