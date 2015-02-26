/*****************************************
 * Harrison Healey - hmhealey - 20376857 *
 *****************************************/

#include "AlgebraToQt.hpp"

#include <QMatrix4x4>

#include "Algebra.hpp"

QMatrix4x4 toQt(const Matrix4& matrix) {
    // for some reason QMatrix4x4's consist of floats in Qt 5.2 and not doubles as they did in previous versions
    float values[16];
    for (int i = 0; i < 16; i++) {
        values[i] = (float) matrix.values[i];
    }

    return QMatrix4x4(values);
}

Matrix4 fromQt(const QMatrix4x4& matrix) {
    return Matrix4(
            matrix(0, 0), matrix(0, 1), matrix(0, 2), matrix(0, 3),
            matrix(1, 0), matrix(1, 1), matrix(1, 2), matrix(1, 3),
            matrix(2, 0), matrix(2, 1), matrix(2, 2), matrix(2, 3),
            matrix(3, 0), matrix(3, 1), matrix(3, 2), matrix(3, 3)
    );
}
