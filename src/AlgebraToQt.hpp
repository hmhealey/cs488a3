#ifndef __ALGEBRA_TO_QT_HPP__
#define __ALGEBRA_TO_QT_HPP__

struct Matrix4;
class QMatrix4x4;

QMatrix4x4 toQt(const Matrix4& matrix);
Matrix4 fromQt(const QMatrix4x4& matrix);

#endif
