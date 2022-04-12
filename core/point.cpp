#include <core/point.h>
#include <core/float4.h>
#include <core/scalar.h>
#include <core/assert.h>
#include <core/vector.h>

namespace rt {

Point::Point(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Point::Point(const Float4& f4)
{
    if (f4.w != 0) {
        x = f4.x / f4.w;
        y = f4.y / f4.w;
        z = f4.z / f4.w;
    }
    else {
        x = f4.x;
        y = f4.y;
        z = f4.z;
    }
}

Vector Point::operator - (const Point& b) const {
    float newX = this->x - b.x;
    float newY = this->y - b.y;
    float newZ = this->z - b.z;

    Vector vec = Vector(newX, newY, newZ);
    return vec;
}

bool Point::operator == (const Point& b) const {
    return (abs(this->x - b.x) <= std::numeric_limits<float>::epsilon()
            && abs(this->y - b.y) <= std::numeric_limits<float>::epsilon()
            && abs(this->z - b.z) <= std::numeric_limits<float>::epsilon());
}

bool Point::operator != (const Point& b) const {
    return !(abs(this->x - b.x) <= std::numeric_limits<float>::epsilon()
        && abs(this->y - b.y) <= std::numeric_limits<float>::epsilon()
        && abs(this->z - b.z) <= std::numeric_limits<float>::epsilon());
}

Point operator * (float scalar, const Point& b) {
    float newX = b.x * scalar;
    float newY = b.y * scalar;
    float newZ = b.z * scalar;

    Point pnt = Point(newX, newY, newZ);
    return pnt;
}

Point operator * (const Point& a, float scalar) {
    float newX = a.x * scalar;
    float newY = a.y * scalar;
    float newZ = a.z * scalar;

    Point pnt = Point(newX, newY, newZ);
    return pnt;
}

Point min(const Point& a, const Point& b) {
    float newX = min(a.x, b.x);
    float newY = min(a.y, b.y);
    float newZ = min(a.z, b.z);

    Point pnt = Point(newX, newY, newZ);
    return pnt;
}

Point max(const Point& a, const Point& b) {
    float newX = max(a.x, b.x);
    float newY = max(a.y, b.y);
    float newZ = max(a.z, b.z);

    Point pnt = Point(newX, newY, newZ);
    return pnt;
}

float dotProduct(const Point& a, const Point& b) {
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}

}
