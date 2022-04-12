#include <core/assert.h>
#include <core/macros.h>
#include <rt/ray.h>

namespace rt {

Ray::Ray(const Point& o, const Vector& d)
{
    this->o = o;
    this->d = d;
}

Point Ray::getPoint(float distance) const {
    return Point(o.x + distance * d.x, o.y + distance * d.y, o.z + distance * d.z);
}

}
