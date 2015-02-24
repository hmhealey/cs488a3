#include "Trackball.hpp"

#include <cmath>

Trackball::Trackball() { }

Trackball::Trackball(const Point2D& center, double radius) : center(center), radius(radius) { }

Vector3 Trackball::windowToTrackball(const Point2D& point) {
    double x = point.x() - center.x();
    double y = point.y() - center.y();

    Vector3 ret(x / radius, y / radius, 0);
    ret.z() = 1.0 - ret.x() * ret.x() - ret.y() * ret.y();

    if (ret.z() < 0) {
        double length = sqrt(1.0 - ret.z());
        ret.z() = 0;
        ret.x() /= length;
        ret.y() /= length;
    } else {
        ret.z() = sqrt(ret.z());
    }

    return ret;
}
