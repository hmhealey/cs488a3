#ifndef __OPTION_HPP__
#define __OPTION_HPP__

#include <QObject>

class BooleanOption : public QObject {
    Q_OBJECT

    bool value;

public:
    BooleanOption();
    BooleanOption(bool value);
    BooleanOption(const BooleanOption& other);

    BooleanOption& operator=(const BooleanOption& other);
    bool& operator=(bool value);

    operator bool() const;

    bool toggle();

private:
    void setValue(bool value);

    friend bool operator==(const BooleanOption& a, const BooleanOption& b);
    friend bool operator==(const BooleanOption& a, bool b);
    friend bool operator==(bool a, const BooleanOption& b);

signals:
    void changed(bool oldValue, bool newValue);
};

bool operator==(const BooleanOption& a, const BooleanOption& b);
bool operator==(const BooleanOption& a, bool b);
bool operator==(bool a, const BooleanOption& b);

#endif
