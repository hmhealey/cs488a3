/*****************************************
 * Harrison Healey - hmhealey - 20376857 *
 *****************************************/

#ifndef __TRACKBALL_HPP__
#define __TRACKBALL_HPP__

#include "Algebra.hpp"

struct Trackball {
    Point2D center;
    double radius;

    Trackball();
    Trackball(const Point2D& center, double radius);

    Vector3 windowToTrackball(const Point2D& point);
};

#endif
