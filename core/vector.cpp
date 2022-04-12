#include <core/scalar.h>
#include <core/vector.h>
#include <core/point.h>
#include <core/float4.h>
#include <core/assert.h>
#include <algorithm>
#include <cmath>

namespace rt {

Vector::Vector(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector::Vector(const Float4& f4)
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

Vector Vector::operator + (const Vector& b) const {
    float newX = this->x + b.x;
    float newY = this->y + b.y;
    float newZ = this->z + b.z;
    Vector vec = Vector(newX, newY, newZ);

    return vec;
}

Vector Vector::operator - (const Vector& b) const {
    float newX = this->x - b.x;
    float newY = this->y - b.y;
    float newZ = this->z - b.z;
    Vector vec = Vector(newX, newY, newZ);

    return vec;
}

Vector Vector::operator - () const {
    float newX = this->x * (-1);
    float newY = this->y * (-1);
    float newZ = this->z * (-1);
    Vector vec = Vector(newX, newY, newZ);

    return vec;
}

Vector Vector::normalize() const {
    float length = this->length();
    float newX = (float)this->x / length;
    float newY = (float)this->y / length;
    float newZ = (float)this->z / length;
    Vector vec = Vector(newX, newY, newZ);

    return vec;
}

Vector operator * (float scalar, const Vector& b) {
    float newX = b.x * scalar;
    float newY = b.y * scalar;
    float newZ = b.z * scalar;
    Vector vec = Vector(newX, newY, newZ);

    return vec;
}

Vector operator * (const Vector& a, float scalar) {
    float newX = a.x * scalar;
    float newY = a.y * scalar;
    float newZ = a.z * scalar;
    Vector vec = Vector(newX, newY, newZ);

    return vec;
}

Vector operator / (const Vector& a, float scalar) {
    float newX = (float)a.x / scalar;
    float newY = (float)a.y / scalar;
    float newZ = (float)a.z / scalar;
    Vector vec = Vector(newX, newY, newZ);

    return vec;
}

Vector cross(const Vector& a, const Vector& b) {
    float newX = a.y * b.z - b.y * a.z;
    float newY = b.x * a.z - a.x * b.z;
    float newZ = a.x * b.y - b.x * a.y;
    Vector vec = Vector(newX, newY, newZ);

    return vec;
}

float dot(const Vector& a, const Vector& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float Vector::lensqr() const {
    return sqr(this->x) + sqr(this->y) + sqr(this->z);
}

float Vector::length() const {
    return sqrt(this->lensqr());
}


bool Vector::operator == (const Vector& b) const {
    return (abs(this->x - b.x) <= std::numeric_limits<float>::epsilon()
            && abs(this->y - b.y) <= std::numeric_limits<float>::epsilon()
            && abs(this->z - b.z) <= std::numeric_limits<float>::epsilon());
}

bool Vector::operator != (const Vector& b) const {
    return !(abs(this->x - b.x) <= std::numeric_limits<float>::epsilon()
        && abs(this->y - b.y) <= std::numeric_limits<float>::epsilon()
        && abs(this->z - b.z) <= std::numeric_limits<float>::epsilon());
}

Vector min(const Vector& a, const Vector& b) {
    float newX = min(a.x, b.x);
    float newY = min(a.y, b.y);
    float newZ = min(a.z, b.z);

    Vector vec = Vector(newX, newY, newZ);
    return vec;
}

Vector max(const Vector& a, const Vector& b) {
    float newX = max(a.x, b.x);
    float newY = max(a.y, b.y);
    float newZ = max(a.z, b.z);

    Vector vec = Vector(newX, newY, newZ);
    return vec;
}

Point operator + (const Point& a, const Vector& b) {
    float newX = a.x + b.x;
    float newY = a.y + b.y;
    float newZ = a.z + b.z;

    Point pnt = Point(newX, newY, newZ);
    return pnt;
}

Point operator + (const Vector& a, const Point& b) {
    float newX = a.x + b.x;
    float newY = a.y + b.y;
    float newZ = a.z + b.z;

    Point pnt = Point(newX, newY, newZ);
    return pnt;
}

Point operator - (const Point& a, const Vector& b) {
    float newX = a.x - b.x;
    float newY = a.y - b.y;
    float newZ = a.z - b.z;

    Point pnt = Point(newX, newY, newZ);
    return pnt;
}

Point operator * (const Float4& scale, const Point& p) {
    /* TODO */ NOT_IMPLEMENTED;
}

}
