#include <rt/bbox.h>
#include <rt/ray.h>
#include <cfloat>

namespace rt {
BBox BBox::empty() {
    BBox newBBox;
    newBBox.mIsEmpty = true;
    return newBBox;
}

BBox BBox::full() {
    BBox newBBox;
    newBBox.min = Point(FLT_MIN, FLT_MIN, FLT_MIN);
    newBBox.max = Point(FLT_MAX, FLT_MAX, FLT_MAX);
    return newBBox;
}


void BBox::extend(const Point& point) {
    if (mIsEmpty)
    {
        mIsEmpty = false;
        min = point;
        max = point;

        return;
    }

    if (min.x > point.x) min.x = point.x;
    if (min.y > point.y) min.y = point.y;
    if (min.z > point.z) min.z = point.z;

    if (max.x < point.x) max.x = point.x;
    if (max.y < point.y) max.y = point.y;
    if (max.z < point.z) max.z = point.z;
}

void BBox::extend(const BBox& bbox) {
    if (mIsEmpty)
    {
        mIsEmpty = false;
        min = bbox.min;
        max = bbox.max;

        return;
    }

    this->extend(bbox.min);
    this->extend(bbox.max);
}

std::pair<float, float> BBox::intersect(const Ray& ray) const {
    float t0x = (min.x - ray.o.x) / ray.d.x;
    float t1x = (max.x - ray.o.x) / ray.d.x;
    if (t0x > t1x) std::swap(t0x, t1x);

    float t0y = (min.y - ray.o.y) / ray.d.y;
    float t1y = (max.y - ray.o.y) / ray.d.y;
    if (t0y > t1y) std::swap(t0y, t1y);

    float t0z = (min.z - ray.o.z) / ray.d.z;
    float t1z = (max.z - ray.o.z) / ray.d.z;
    if (t0z > t1z) std::swap(t0z, t1z);

    float maxt0 = std::max(std::max(t0x, t0y), t0z);

    float mint1 = std::min(std::min(t1x, t1y), t1z);

    std::pair<float, float> p;
    p = std::make_pair(maxt0, mint1);
    return p;
}

bool BBox::isUnbound() const {
    if (min.x == FLT_MIN) return true;
    if (min.y == FLT_MIN) return true;
    if (min.z == FLT_MIN) return true;

    if (max.x == FLT_MAX) return true;
    if (max.y == FLT_MAX) return true;
    if (max.z == FLT_MAX) return true;

    return false;
}

}