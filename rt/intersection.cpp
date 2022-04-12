#include <rt/intersection.h>

namespace rt {

Intersection::Intersection(float distance, const Ray& ray, const Solid* solid, const Vector& normal, const Point& local)
{
    /* TODO */

    this->distance = distance;
    this->ray = ray;
    this->solid = solid;
    this->mNormal = normal;
    this->mLocal = local;
    this->isintersec = true;
}

Intersection::operator bool() const {
    /* TODO */ 
    return isintersec;
}

Intersection Intersection::failure() {
    /* TODO */ 
    Intersection isec;
    isec.isintersec = false;
    return isec;
}

Point Intersection::hitPoint() const {
    /* TODO */ 
    return ray.getPoint(distance);
}

Vector Intersection::normal() const {
    /* TODO */ 
    return mNormal;
}

Point Intersection::local() const {
    /* TODO */ 
    return mLocal;
}

}
