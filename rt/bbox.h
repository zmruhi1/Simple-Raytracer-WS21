#ifndef CG1RAYTRACER_BBOX_HEADER
#define CG1RAYTRACER_BBOX_HEADER

#include <utility>
#include <core/point.h>
#include <core/vector.h>

namespace rt {

class Ray;

class BBox {
public:
    Point min, max;

    BBox() {}
    BBox(const Point& min, const Point& max)
    {
        /* TODO */
        this->min = min;
        this->max = max;
        mIsEmpty = false;
    }

    static BBox empty();
    static BBox full();

    void extend(const Point& point);
    void extend(const BBox& bbox);

    Vector diagonal() const {
        return max - min;
    }

    float area() const {
        return (max.x - min.x) * (max.y - min.y) * (max.z - min.z);
    }

    std::pair<float, float> intersect(const Ray& ray) const;

    bool isUnbound() const;
    bool mIsEmpty;
};

}

#endif