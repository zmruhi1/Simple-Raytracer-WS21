#include <rt/solids/infiniteplane.h>
#include <cfloat>

namespace rt {

InfinitePlane::InfinitePlane(const Point& origin, const Vector& normal, CoordMapper* texMapper, Material* material)
{
    /* TODO */
    this->mOrigin = origin;
    this->mNormal = normal.normalize();
    setMaterial(material);
    setCoordMapper(texMapper);

    mBBox = BBox::full();
}

BBox InfinitePlane::getBounds() const {
    /* TODO */ 
    return mBBox;
}

Intersection InfinitePlane::intersect(const Ray& ray, float previousBestDistance) const {
    float denom = dot(ray.d, this->mNormal);
    if (fabsf(denom) <= epsilon)
        return Intersection::failure(); 

    float t = -dot(ray.o - mOrigin, mNormal) / denom;

    if (t < epsilon) return Intersection::failure();

    if (t < previousBestDistance) {
        Point surfacePoint = ray.getPoint(t);
        return Intersection(t, ray, this, mNormal, surfacePoint);
    }
}

Solid::Sample InfinitePlane::sample() const {
    /* TODO */ NOT_IMPLEMENTED;
}

float InfinitePlane::getArea() const {
    /* TODO */ 
    return FLT_MAX;
}

}
